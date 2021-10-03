#ifndef VK_EVENTS_WALL_REPLY_NEW_HPP
#define VK_EVENTS_WALL_REPLY_NEW_HPP

#include "cpp_vk_lib/vk/attachment/attachment.hpp"

#include <vector>

namespace simdjson::dom {
class object;
}// namespace simdjson::dom

namespace vk::event {
/*!
 * Internal information accessed in a "lazy way".
 * It means, that no data is extracted from JSON
 * until the user wants to access it
 */
class wall_reply_new
{
public:
    explicit wall_reply_new(simdjson::dom::object event);
    ~wall_reply_new();

    int64_t id() const noexcept;
    int64_t from_id() const noexcept;
    int64_t post_id() const noexcept;
    int64_t owner_id() const noexcept;
    std::string text() const noexcept;
    bool has_attachments() const noexcept;
    /*!
     * \note If no attachments were provided, empty vector returned
     */
    std::vector<attachment::attachment_ptr_t> attachments() const;

private:
    const simdjson::dom::object& get_event() const;

    std::unique_ptr<simdjson::dom::object> event_json_;
    bool has_attachments_ = false;
};

std::ostream& operator<<(std::ostream& ostream, const vk::event::wall_reply_new& reply);

}// namespace vk::event

#endif// VK_EVENTS_WALL_REPLY_NEW_HPP
