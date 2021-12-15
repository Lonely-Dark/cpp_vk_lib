#ifndef VK_EVENTS_WALL_POST_NEW_HPP
#define VK_EVENTS_WALL_POST_NEW_HPP

#include "cpp_vk_lib/vk/events/wall_repost.hpp"

namespace simdjson::dom {
class object;
}// namespace simdjson::dom

namespace vk::event {
/*!
 * Internal information accessed in a "lazy way".
 * It means, that no data is extracted from JSON until the user
 * wants to access it
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
     * \throws vk::exception::access_error if repost doesn't exist
     */
    std::unique_ptr<wall_repost> repost() const;
    /*!
     * \throws vk::exception::access_error if no attachments were provided
     */
    std::vector<vk::attachment::attachment_ptr_t> attachments() const;

    /*! Friend declaration really needed since the internal JSON is checked. */
    friend std::ostream& operator<<(std::ostream&, const wall_post_new&);

private:
    const simdjson::dom::object& get_event() const;

    std::unique_ptr<simdjson::dom::object> event_json_;
    bool has_attachments_ = false;
    bool has_repost_      = false;
};

std::ostream& operator<<(std::ostream& ostream, const wall_post_new& event);

}// namespace vk::event

#endif// VK_EVENTS_WALL_POST_NEW_HPP
