#include "cpp_vk_lib/vk/events/common.hpp"

#include "cpp_vk_lib/vk/events/message_new.hpp"
#include "cpp_vk_lib/vk/events/wall_post_new.hpp"
#include "cpp_vk_lib/vk/events/wall_reply_new.hpp"
#include "simdjson.h"

namespace vk::event {

common::~common() = default;

common::common(std::string_view ts, simdjson::dom::object event)
    : ts_(ts)
    , underlying_event_type_()
    , event_json_(std::make_unique<simdjson::dom::object>(event))
{
    std::string_view type(get_event()["type"]);
    if (type == "message_new" || type == "message_reply") {
        underlying_event_type_ = event::type::message_new;
    } else if (type == "wall_post_new") {
        underlying_event_type_ = event::type::wall_post_new;
    } else if (type == "wall_reply_new") {
        underlying_event_type_ = event::type::wall_reply_new;
    } else if (type == "wall_repost") {
        underlying_event_type_ = event::type::wall_repost;
    } else {
        const std::string errmsg = "unknown event type: " + std::string(type);
        throw error::runtime_error(-1, errmsg.c_str());
    }
}

const simdjson::dom::object& common::get_event() const noexcept
{
    return *event_json_;
}

bool common::on_type(event::type type) const noexcept
{
    return underlying_event_type_ == type;
}

message_new common::get_message_new() const
{
    simdjson::dom::object object = get_event()["object"];
    simdjson::dom::object message;
    if (auto error = object["message"].get(message); !error) {
        return message_new(message);
    } else {
        return message_new(object);
    }
}

wall_post_new common::get_wall_post_new() const
{
    return wall_post_new(std::move(get_event()["object"]));
}

wall_reply_new common::get_wall_reply_new() const
{
    return wall_reply_new(std::move(get_event()["object"]));
}

event::type common::type() const noexcept
{
    return underlying_event_type_;
}

const std::string& common::ts() const noexcept
{
    return ts_;
}

std::string common::dump() const
{
    return simdjson::to_string(get_event());
}

}// namespace vk::event
