#ifndef VK_EVENTS_MESSAGE_NEW_HPP
#define VK_EVENTS_MESSAGE_NEW_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"

#include <any>
#include <vector>

namespace simdjson::dom {
class object;
class array;
}// namespace simdjson::dom

namespace vk::event {
/*!
 * Internal information accessed in a "lazy way".
 * It means, that no data is extracted from JSON until the user
 * wants to access it, and there's meaningless to construct all
 * attachments, reply and forwarded messages objects in the case
 * you only need message text.
 */
class message_new
{
public:
    message_new(simdjson::dom::object);
    ~message_new();
    /*!
     * \throws vk::exception::access_error if reply pointer is not set
     */
    std::unique_ptr<message_new> reply() const;
    /*!
     * \throws vk::exception::access_error if forward messages vector is empty
     */
    std::vector<std::unique_ptr<message_new>> fwd_messages() const;
    /*!
     * \throws vk::exception::access error if there's no actions in event
     */
    std::any action() const;
    /*!
     * \throws exception::access_error if object hasn't attachments
     * \return attachments from event if any, empty vector otherwise
     */
    std::vector<attachment::attachment_ptr_t> attachments() const;

    bool on_action(std::string_view action_type) const noexcept;

    int64_t conversation_message_id() const noexcept;
    std::string text() const noexcept;
    int64_t from_id() const noexcept;
    int64_t peer_id() const noexcept;
    bool has_reply() const noexcept;
    bool has_fwd_messages() const noexcept;
    bool has_action() const noexcept;

private:
    void try_get_actions();
    simdjson::dom::object& event() const;

    std::unique_ptr<simdjson::dom::object> event_json_;
    std::any action_;
    bool has_action_       = false;
    bool has_reply_        = false;
    bool has_fwd_messages_ = false;
    bool has_attachments_  = false;
};

std::ostream& operator<<(std::ostream&, const vk::event::message_new&);

}// namespace vk::event

#endif// VK_EVENTS_MESSAGE_NEW_HPP
