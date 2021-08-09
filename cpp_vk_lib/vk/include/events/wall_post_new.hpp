#ifndef VK_EVENTS_WALL_POST_NEW_HPP
#define VK_EVENTS_WALL_POST_NEW_HPP

#include "vk/include/events/wall_repost.hpp"

namespace simdjson {
namespace dom {
class object;
}// namespace dom
}// namespace simdjson

namespace vk {
namespace event {
/*!
 * @brief The `wall_post_new` event representation.
 *
 * Internal information accessed in a "lazy way".
 * It means, that no data is extracted from JSON until the user wants to access it.
 */
class wall_post_new
{
public:
    ~wall_post_new();

    wall_post_new(simdjson::dom::object event);

    int64_t id() const noexcept;
    int64_t from_id() const noexcept;
    int64_t owner_id() const noexcept;
    int64_t created_by() const noexcept;
    std::string text() const noexcept;
    bool can_edit() const noexcept;
    bool can_delete() const noexcept;
    bool marked_as_ads() const noexcept;
    bool has_attachments() const noexcept;
    bool has_repost() const noexcept;
    /*!
     * @throws vk::exception::access_error in case, when _repost pointer is not set.
     */
    std::shared_ptr<wall_repost> repost() const;
    /*!
     * @brief Get attachments vector.
     * @note In case, when no attachments were provided, empty vector returned.
     */
    std::vector<vk::attachment::attachment_ptr_t> attachments() const;

private:
    simdjson::dom::object& get_event() const;

    std::shared_ptr<simdjson::dom::object> event_json_;
    bool has_attachments_ = false;
    bool has_repost_ = false;
};

}// namespace event
}// namespace vk

std::ostream& operator<<(std::ostream& ostream, const vk::event::wall_post_new& event);

#endif// VK_EVENTS_WALL_POST_NEW_HPP
