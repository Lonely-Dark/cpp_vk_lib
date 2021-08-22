#include "cpp_vk_lib/vk/methods/basic.hpp"

#include "cpp_vk_lib/vk/exception/error-inl.hpp"
#include "cpp_vk_lib/vk/methods/utility/message_constructor.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

namespace vk::method {

void messages::send(int64_t peer_id, std::string_view text, bool mentions_flag)
{
    spdlog::trace(
        "call messages::send: peer_id={}, text={}, mentions_flag={}",
        peer_id,
        text,
        mentions_flag);

    message_constructor constructor(mentions_flag);

    constructor.param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .perform_request();
}

void messages::send(
    int64_t peer_id,
    std::string_view text,
    std::vector<attachment::attachment_ptr_t> list,
    bool mentions_flag)
{
    spdlog::trace(
        "call messages::send: peer_id={}, text={}, attachments_count={}, mentions_flag={}",
        peer_id,
        text,
        list.size(),
        mentions_flag);

    message_constructor constructor(mentions_flag);

    constructor.param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .attachments(std::move(list))
        .perform_request();
}

void messages::send(
    int64_t peer_id,
    std::string_view text,
    std::string_view keyboard_layout,
    bool mentions_flag)
{
    spdlog::trace(
        "call messages::send: peer_id={}, text={}, keyboard={}, mentions_flag={}",
        peer_id,
        text,
        keyboard_layout,
        mentions_flag);

    message_constructor constructor(mentions_flag);

    constructor.param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .param("keyboard", keyboard_layout)
        .perform_request();
}

int64_t groups::get_by_id(error_code& errc)
{
    spdlog::trace("call groups::get_by_id");

    const std::string response =
        group_constructor().method("groups.getById").perform_request();

    simdjson::dom::parser parser;
    const simdjson::dom::object parsed = parser.parse(response);

    if (parsed.begin().key() == "error") {
        errc.assign(exception::translate_error(
            parsed["error"]["error_code"].get_int64()));
        return -1;
    }

    return parsed["response"].at(0)["id"];
}

std::string groups::get_long_poll_server(int64_t group_id)
{
    spdlog::trace("call groups::get_long_poll_server: group_id={}", group_id);

    return group_constructor()
        .method("groups.getLongPollServer")
        .param("group_id", std::to_string(group_id))
        .param("random_id", "0")
        .perform_request();
}

}// namespace vk::method
