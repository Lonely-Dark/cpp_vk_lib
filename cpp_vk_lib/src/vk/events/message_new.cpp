#include "cpp_vk_lib/vk/events/message_new.hpp"

#include "cpp_vk_lib/runtime/misc/cppdefs.hpp"
#include "cpp_vk_lib/vk/events/attachment_handler.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

#include <iomanip>

namespace vk::event {

message_new::~message_new() = default;

message_new::message_new(simdjson::dom::object event)
    : event_json_(std::make_shared<simdjson::dom::object>(event))
    , action_()
{
    if (get_event()["reply_message"].is_object()) {
        has_reply_ = true;
    }

    if (get_event()["attachments"].is_array()) {
        has_attachments_ = true;
    }

    if (get_event()["fwd_messages"].is_array() &&
        get_event()["fwd_messages"].get_array().size() != 0) {
        has_fwd_messages_ = true;
    }

    if (get_event()["action"].is_object()) {
        has_action_ = true;
        try_get_actions();
    }

    if (spdlog::get_level() == SPDLOG_LEVEL_TRACE) {
        std::ostringstream ostream;
        ostream << "creating an event ";
        ostream << *this;
        spdlog::trace("{}", ostream.str());
    }
}

void message_new::try_get_actions()
{
    simdjson::dom::object action = get_event()["action"].get_object();
    std::string action_name = action["type"].get_string().take_value().data();

    if (action_name == "chat_invite_user") {
        action_ = action::chat_invite_user{action["member_id"].get_int64()};
    }

    if (action_name == "chat_kick_user") {
        action_ = action::chat_kick_user{action["member_id"].get_int64()};
    }

    if (action_name == "chat_pin_message") {
        action_ = action::chat_pin_message{
            action["member_id"].get_int64(),
            action["conversation_message_id"].get_int64(),
            action["message"].get_c_str().take_value()};
    }

    if (action_name == "chat_unpin_message") {
        action_ = action::chat_unpin_message{
            action["member_id"].get_int64(),
            action["conversation_message_id"].get_int64()};
    }

    if (action_name == "chat_photo_update") {
        action_ = action::chat_photo_update{
            // empty
        };
    }

    if (action_name == "chat_title_update") {
        action_ = action::chat_title_update{action["text"].get_c_str().take_value()};
    }
}

template <typename T>
static VK_REALLY_INLINE bool check_action(const std::any& action) noexcept
{
    try {
        std::any_cast<T>(action);
    } catch (std::bad_any_cast& err) {
        return false;
    }
    return true;
}

bool message_new::on_action(std::string_view action_type) const noexcept
{
    if (action_type == "chat_invite_user") {
        return check_action<action::chat_invite_user>(action_);
    }

    if (action_type == "chat_kick_user") {
        return check_action<action::chat_kick_user>(action_);
    }

    if (action_type == "chat_pin_message") {
        return check_action<action::chat_pin_message>(action_);
    }

    if (action_type == "chat_unpin_message") {
        return check_action<action::chat_unpin_message>(action_);
    }

    if (action_type == "chat_photo_update") {
        return check_action<action::chat_photo_update>(action_);
    }

    if (action_type == "chat_title_update") {
        return check_action<action::chat_title_update>(action_);
    }

    return false;
}

simdjson::dom::object& message_new::get_event() const
{
    return *event_json_;
}

int64_t message_new::conversation_message_id() const noexcept
{
    return get_event()["conversation_message_id"].get_int64();
}

int64_t message_new::peer_id() const noexcept
{
    return get_event()["peer_id"].get_int64();
}

int64_t message_new::from_id() const noexcept
{
    return get_event()["from_id"].get_int64();
}

std::string message_new::text() const noexcept
{
    return get_event()["text"].get_c_str().take_value();
}

bool message_new::has_action() const noexcept
{
    return has_action_;
}

bool message_new::has_reply() const noexcept
{
    return has_reply_;
}

bool message_new::has_fwd_messages() const noexcept
{
    return has_fwd_messages_;
}

std::any message_new::action() const
{
    if (has_action_) {
        return action_;
    } else {
        throw error::access_error(-1, "attempting accessing empty action");
    }
}

std::vector<attachment::attachment_ptr_t> message_new::attachments() const
{
    if (has_attachments_) {
        return event::get_attachments(get_event()["attachments"].get_array());
    } else {
        throw error::access_error(-1, "attempting accessing empty attachment list");
    }
}

std::vector<std::unique_ptr<message_new>> message_new::fwd_messages() const
{
    if (has_attachments_) {
        std::vector<std::unique_ptr<message_new>> fwd_messages;

        for (simdjson::dom::element fwd_message : get_event()["fwd_messages"].get_array()) {
            fwd_messages.emplace_back(std::make_unique<message_new>(fwd_message));
        }

        return fwd_messages;
    } else {
        throw error::access_error(-1, "attempting accessing empty forward messages list");
    }
}

std::shared_ptr<message_new> message_new::reply() const
{
    if (has_reply_) {
        return std::make_unique<message_new>(get_event()["reply_message"].get_object());
    } else {
        throw error::access_error(-1, "attempting accessing empty reply");
    }
}

void dispatch_events(std::ostream& ostream, const message_new& event)
{
    if (event.has_action()) {
        if (event.on_action("chat_invite_user")) {
            ostream << std::setw(40) << "chat_invite_user action: ";
            ostream << std::any_cast<action::chat_invite_user>(event.action()).member_id;
            ostream << std::endl;
        }

        if (event.on_action("chat_kick_user")) {
            ostream << std::setw(40) << "chat_kick_user action: ";
            ostream << std::any_cast<action::chat_kick_user>(event.action()).member_id;
            ostream << std::endl;
        }

        if (event.on_action("chat_pin_message")) {
            ostream << std::setw(40) << "chat_pin_message action: ";
            ostream << std::any_cast<action::chat_pin_message>(event.action()).member_id;
            ostream << std::endl;
        }

        if (event.on_action("chat_unpin_message")) {
            ostream << std::setw(40) << "chat_unpin_message action: ";
            ostream << std::any_cast<action::chat_unpin_message>(event.action()).member_id;
            ostream << std::endl;
        }

        if (event.on_action("chat_photo_update")) {
            ostream << std::setw(40) << "chat_photo_update action:  ";
            ostream << "<empty>";
            ostream << std::endl;
        }

        if (event.on_action("chat_title_update")) {
            ostream << std::setw(30) << "chat_title_update action:  ";
            ostream << std::any_cast<action::chat_title_update>(event.action()).text;
            ostream << std::endl;
        }
    }
}

std::ostream& operator<<(std::ostream& ostream, const message_new& event)
{
    ostream << "message_new:" << std::endl;

    ostream << std::setw(30) << "conversation_message_id: " << event.conversation_message_id()
            << std::endl;
    ostream << std::setw(30) << "peer_id: " << event.peer_id() << std::endl;
    ostream << std::setw(30) << "from_id: " << event.from_id() << std::endl;
    ostream << std::setw(30) << "text: " << event.text() << std::endl;
    ostream << std::setw(30) << "has_action: " << event.has_action() << std::endl;
    ostream << std::setw(30) << "has_reply: " << event.has_reply() << std::endl;
    ostream << std::setw(30) << "has_fwd_messages: " << event.has_fwd_messages() << std::endl;

    if (event.has_reply()) {
        ostream << std::endl << std::setw(30) << "reply: " << *event.reply() << std::endl;
    }

    dispatch_events(ostream, event);

    for (auto& attachment : event.attachments()) {
        ostream << std::setw(30) << "attachment: ";
        ostream << attachment->value();
        ostream << std::endl;
    }

    if (event.has_fwd_messages()) {
        for (auto& message : event.fwd_messages()) {
            ostream << std::endl << std::setw(30) << "fwd_message: ";
            ostream << *message;
            ostream << std::endl;
        }
    }

    return ostream;
}

}// namespace vk::event
