#include "cpp_vk_lib/runtime/string_utils/string_utils.hpp"
#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/runtime/signal_handlers.hpp"
#include "cpp_vk_lib/vk/long_poll/long_poll.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/events/message_new.hpp"
#include "cpp_vk_lib/vk/methods/basic.hpp"

#include <iostream>

inline bool cpp_vk_lib_curl_verbose = false;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./long_poll <config.json>" << std::endl;
        return 1;
    }

    vk::config::load(argv[1]);
    runtime::setup_signal_handlers();
    runtime::setup_logger(spdlog::level::level_enum::trace);

    asio::io_context io_context;
    vk::long_poll api(io_context);

    while (true) {
        auto events = api.listen();
        for (const auto& event : events) {
            api.on_event("message_new", event, [&event] {
                vk::event::message_new message_event = event.get_message_new();
                vk::method::messages::send(message_event.peer_id(), "response");
            });
        }
        api.run();
    }
    return 0;
}
