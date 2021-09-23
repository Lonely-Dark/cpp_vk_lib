#ifndef VK_EVENTS_COMMON_HPP
#define VK_EVENTS_COMMON_HPP

#include "cpp_vk_lib/vk/events/type.hpp"

#include <memory>
#include <string>

namespace vk::event {
class message_new;
class wall_post_new;
class wall_reply_new;
}// namespace vk::event

namespace simdjson::dom {
class object;
}// namespace simdjson::dom

namespace vk::event {
/*!
 * Temporary update container used to store any event represented
 * in raw JSON. Use the appropriate functions to get needed event
 * types
 */
class common
{
public:
    common(std::string_view ts, simdjson::dom::object event);
    ~common();

    event::type type() const noexcept;
    std::string ts() const noexcept;
    std::string dump() const noexcept;

    bool on_type(event::type) const noexcept;

    //    explicit operator message_new() const;
    //    explicit operator wall_post_new() const;
    //    explicit operator wall_reply_new() const;

    message_new get_message_new() const;
    wall_post_new get_wall_post_new() const;
    wall_reply_new get_wall_reply_new() const;

private:
    simdjson::dom::object& get_event() const noexcept;

    std::string ts_;
    event::type underlying_event_type_;
    std::shared_ptr<simdjson::dom::object> event_;
};

}// namespace vk::event

#endif// VK_EVENTS_COMMON_EVENT_HPP
