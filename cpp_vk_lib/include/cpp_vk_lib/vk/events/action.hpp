#ifndef VK_EVENTS_HANDLERS_MESSAGE_ACTION_HANDLER_HPP
#define VK_EVENTS_HANDLERS_MESSAGE_ACTION_HANDLER_HPP

#include <string>

namespace vk::action {

// clang-format off
struct chat_invite_user { int64_t member_id; };
struct chat_kick_user { int64_t member_id; };
struct chat_pin_message { int64_t member_id; std::string message; };
struct chat_unpin_message { int64_t member_id; };
struct chat_photo_update { /* Empty. */ };
struct chat_title_update { std::string text; };
// clang-format on

}// namespace vk::action

#endif// VK_EVENTS_HANDLERS_MESSAGE_ACTION_HANDLER_HPP
