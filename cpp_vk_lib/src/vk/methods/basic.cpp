#include "cpp_vk_lib/vk/methods/basic.hpp"

#include "cpp_vk_lib/vk/error/translate_error.hpp"
#include "cpp_vk_lib/vk/methods/message_constructor.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

template <typename... Args>
static void perform_messages_method_log(std::string_view fmt, int64_t peer_id, std::string_view text, Args&&... args)
{
    if (spdlog::get_level() > spdlog::level::debug) {
        return;
    }

    std::string one_line_text = text.data();
    std::replace(one_line_text.begin(), one_line_text.end(), '\n', ' ');
    spdlog::debug(fmt, peer_id, one_line_text, std::forward<Args>(args)...);
}

namespace vk::method {

void messages::send(int64_t peer_id, std::string_view text, enum mentions mentions)
{
    perform_messages_method_log(
        "call messages::send: peer_id={}, text={}, mentions_flag={}",
        peer_id, text, to_string(mentions)
    );

    message_constructor(mentions)
        .param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .request_without_output();
}

void messages::send(int64_t peer_id, std::string_view text, const std::vector<attachment::attachment_ptr_t>& list, enum mentions mentions)
{
    perform_messages_method_log(
        "call messages::send: peer_id={}, text={}, attachments_count={}, mentions_flag={}",
        peer_id, text, list.size(), to_string(mentions)
    );

    message_constructor(mentions)
        .param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .attachments(list)
        .request_without_output();
}

void messages::send(int64_t peer_id, std::string_view text, const attachment::attachment_ptr_t& attachment, enum mentions mentions)
{
    perform_messages_method_log(
        "call messages::send: peer_id={}, text={}, attachment={}, mentions_flag={}",
        peer_id, text, attachment->value(), to_string(mentions)
    );

    message_constructor(mentions)
        .param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .attachment(attachment)
        .request_without_output();
}

void messages::send(int64_t peer_id, std::string_view text, std::string_view keyboard_layout, enum mentions mentions)
{
    perform_messages_method_log(
        "call messages::send: peer_id={}, text={}, keyboard={}, mentions_flag={}",
        peer_id, text, keyboard_layout, to_string(mentions)
    );

    message_constructor(mentions)
        .param("peer_id", std::to_string(peer_id))
        .param("message", text)
        .param("keyboard", keyboard_layout)
        .request_without_output();
}

int64_t groups::get_by_id(error_code& errc)
{
    spdlog::debug("call groups::get_by_id");

    const std::string response = group_constructor().method("groups.getById").perform_request();

    simdjson::dom::parser parser;
    const simdjson::dom::object parsed = parser.parse(response);

    if (parsed.begin().key() == "error") {
        errc.assign(error::translate_to_string(parsed["error"]["error_code"].get_int64()));
        return -1;
    }

    return parsed["response"].at(0)["id"];
}

std::string groups::get_long_poll_server(int64_t group_id)
{
    spdlog::debug("call groups::get_long_poll_server: group_id={}", group_id);

    return group_constructor()
        .method("groups.getLongPollServer")
        .param("group_id", std::to_string(group_id))
        .param("random_id", "0")
        .perform_request();
}

}// namespace vk::method
