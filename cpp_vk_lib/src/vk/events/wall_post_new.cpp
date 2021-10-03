#include "cpp_vk_lib/vk/events/wall_post_new.hpp"

#include "cpp_vk_lib/vk/events/attachment_handler.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

namespace vk::event {

wall_post_new::~wall_post_new() = default;

wall_post_new::wall_post_new(simdjson::dom::object event)
    : event_json_(std::make_unique<simdjson::dom::object>(event))
{
    if (get_event()["attachments"].is_array()) {
        has_attachments_ = true;
    }

    if (get_event()["copy_history"].is_array()) {
        has_repost_ = true;
    }

    if (spdlog::get_level() == SPDLOG_LEVEL_TRACE) {
        std::ostringstream ostream;
        ostream << "creating an event ";
        ostream << *this;
        spdlog::trace("{}", ostream.str());
    }
}

int64_t wall_post_new::id() const noexcept
{
    return get_event()["id"].get_int64();
}

int64_t wall_post_new::from_id() const noexcept
{
    return get_event()["from_id"].get_int64();
}

int64_t wall_post_new::owner_id() const noexcept
{
    return get_event()["owner_id"].get_int64();
}

int64_t wall_post_new::created_by() const noexcept
{
    return get_event()["created_by"].get_int64();
}

std::string wall_post_new::text() const noexcept
{
    return get_event()["text"].get_string().take_value().data();
}

bool wall_post_new::can_edit() const noexcept
{
    return get_event()["can_edit"].get_int64();
}

bool wall_post_new::can_delete() const noexcept
{
    return get_event()["can_delete"].get_int64();
}

bool wall_post_new::marked_as_ads() const noexcept
{
    return get_event()["marked_as_ads"].get_int64();
}

bool wall_post_new::has_attachments() const noexcept
{
    return has_attachments_;
}

bool wall_post_new::has_repost() const noexcept
{
    return has_repost_;
}

const simdjson::dom::object& wall_post_new::get_event() const
{
    return *event_json_;
}

std::vector<vk::attachment::attachment_ptr_t> wall_post_new::attachments() const
{
    if (has_attachments_) {
        return event::get_attachments(get_event()["attachments"]);
    } else {
        throw error::access_error(-1, "Attempting accessing empty attachment list.");
    }
}

std::unique_ptr<wall_repost> wall_post_new::repost() const
{
    simdjson::dom::object repost_json = get_event()["copy_history"].get_array().at(0).get_object();

    if (has_repost_) {
        std::unique_ptr<wall_repost> repost = std::make_unique<wall_repost>(
            repost_json["id"],
            repost_json["from_id"],
            repost_json["owner_id"],
            repost_json["text"].get_c_str().take_value());

        if (repost_json["attachments"].is_array() &&
            repost_json["attachments"].get_array().size() > 0) {
            repost->construct_attachments(event::get_attachments(repost_json["attachments"]));
        }

        return repost;
    } else {
        throw error::access_error(-1, "Attempting accessing empty repost");
    }
}

std::ostream& operator<<(std::ostream& ostream, const vk::event::wall_post_new& event)
{
    ostream << "wall_post_new:" << std::endl;

    ostream << std::setw(30) << "id: " << event.id() << std::endl;
    ostream << std::setw(30) << "from_id: " << event.from_id() << std::endl;
    ostream << std::setw(30) << "owner_id: " << event.owner_id() << std::endl;
    ostream << std::setw(30) << "text: " << event.text() << std::endl;
    ostream << std::setw(30) << "marked_as_ads? " << std::boolalpha << event.marked_as_ads()
            << std::endl;

    int64_t can_delete;
    if (auto error = event.get_event()["can_delete"].get(can_delete); !error) {
        ostream << std::setw(30) << "can_delete?: " << can_delete << std::endl;
    } else {
        /* Ignore it. */
    }

    int64_t created_by;
    if (auto error = event.get_event()["created_by"].get(created_by); !error) {
        ostream << std::setw(30) << "created_by: " << created_by << std::endl;
    } else {
        /* Ignore it. */
    }

    int64_t can_edit;
    if (auto error = event.get_event()["can_edit"].get(can_edit); !error) {
        ostream << std::setw(30) << "can_edit: " << created_by << std::endl;
    } else {
        /* Ignore it. */
    }

    auto append_attachments = [&ostream](const auto& attachments) {
        for (auto& attachment : attachments) {
            ostream << std::setw(30) << "attachment: ";
            ostream << attachment->value();
            ostream << std::endl;
        }
    };

    if (event.has_attachments()) {
        append_attachments(event.attachments());
    }

    if (event.has_repost()) {
        ostream << std::endl << std::setw(30) << "repost:";
        ostream << "from_id: " << event.repost()->from_id() << std::endl;
        ostream << std::setw(30) << "id: " << event.repost()->id() << std::endl;
        ostream << std::setw(30) << "owner_id: " << event.repost()->owner_id() << std::endl;
        ostream << std::setw(30) << "text: " << event.repost()->text() << std::endl;

        if (!event.repost()->attachments().empty()) {
            append_attachments(event.repost()->attachments());
        }
    }

    return ostream;
}

}// namespace vk::event
