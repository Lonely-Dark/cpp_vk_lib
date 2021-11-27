#include "cpp_vk_lib/vk/long_poll/long_poll.hpp"

#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/error/translate_error.hpp"
#include "cpp_vk_lib/vk/methods/basic.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

namespace vk {

long_poll::long_poll(asio::io_context& io_context)
    : parser_(std::make_unique<simdjson::dom::parser>())
    , executors_()
    , poll_payload_()
    , errc_()
    , io_context_(io_context)
{
    group_id_ = method::groups::get_by_id(errc_);
    if (errc_) { throw error::access_error(-1, "error retrieve group id"); }
    spdlog::info("long poll group: {}", group_id_);
}

long_poll::~long_poll() = default;

long_poll::poll_payload long_poll::server() const
{
    const std::string data                    = method::groups::get_long_poll_server(group_id_);
    const simdjson::dom::object server_object = parser_->parse(data)["response"];
    return {std::string(server_object["key"]), std::string(server_object["server"]), std::string(server_object["ts"])};
}

std::vector<event::common> long_poll::listen(int8_t timeout)
{
    static bool new_server_needed = true;
    if (new_server_needed) {
        poll_payload_     = server();
        new_server_needed = false;
    }

    spdlog::trace("long poll: ts {}, timeout {}", poll_payload_.ts, timeout);

    // clang-format off
    const std::string response = method::raw_constructor()
        .method(poll_payload_.server + "?")
        .param("act",  "a_check")
        .param("key",  poll_payload_.key)
        .param("ts",   poll_payload_.ts)
        .param("wait", std::to_string(timeout))
        .perform_request();
    // clang-format on

    const simdjson::dom::object parsed_response = parser_->parse(response);

    if (parsed_response.begin().key() == "failed") {
        const int64_t code = parsed_response["failed"].get_int64();
        if (code == 2 || code == 3) {
            new_server_needed = true;
            listen(timeout);
        }
    }

    std::vector<event::common> event_list;
    std::string ts(parsed_response["ts"]);

    for (auto update : parsed_response["updates"]) {
        if (std::string_view(update["type"]) == "message_typing_state") { continue; }
        simdjson::dom::object object = update["object"];
        simdjson::dom::object message;
        if (auto error = object["message"].get(message); error) {
            if (object["from_id"].get_int64() == group_id_ * -1) { continue; }
        }
        event_list.emplace_back(ts, update);
    }
    poll_payload_.ts = std::move(ts);

    return event_list;
}

void long_poll::run(int8_t timeout)
{
    while (true) {
        std::vector<vk::event::common> events = listen(timeout);
        for (const auto& event : events) {
            asio::post(io_context_, [this, &event]() mutable {
                executors_[event.type()](event);
            });
        }
        const size_t num_workers = config::num_workers();
        std::vector<std::thread> threads;
        threads.reserve(num_workers);
        for (size_t i = 0; i < num_workers; ++i) {
            threads.emplace_back([this] {
                io_context_.run();
            });
        }
        io_context_.run();
        for (auto& t : threads) { t.join(); }
        io_context_.restart();
    }
}

}// namespace vk
