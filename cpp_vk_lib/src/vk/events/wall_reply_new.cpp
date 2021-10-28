#include "cpp_vk_lib/vk/events/wall_reply_new.hpp"

#include "cpp_vk_lib/vk/events/attachment_handler.hpp"
#include "simdjson.h"
#include "spdlog/spdlog.h"

namespace vk::event {

wall_reply_new::~wall_reply_new() = default;

wall_reply_new::wall_reply_new(simdjson::dom::object event)
    : event_json_(std::make_unique<simdjson::dom::object>(event))
{
    if (get_event()["attachments"].is_array() &&
        get_event()["attachments"].get_array().size() > 0) {
        has_attachments_ = true;
    }

    if (spdlog::get_level() == SPDLOG_LEVEL_TRACE) {
        std::ostringstream ostream;
        ostream << "creating an event ";
        ostream << *this;
        spdlog::trace("{}", ostream.str());
    }
}

const simdjson::dom::object& wall_reply_new::get_event() const
{
    return *event_json_;
}

int64_t wall_reply_new::id() const noexcept
{
    return get_event()["id"].get_int64();
}

int64_t wall_reply_new::from_id() const noexcept
{
    return get_event()["from_id"].get_int64();
}

int64_t wall_reply_new::post_id() const noexcept
{
    return get_event()["post_id"].get_int64();
}

int64_t wall_reply_new::owner_id() const noexcept
{
    return get_event()["owner_id"].get_int64();
}

std::string wall_reply_new::text() const noexcept
{
    return get_event()["text"].get_c_str().take_value();
}

bool wall_reply_new::has_attachments() const noexcept
{
    return has_attachments_;
}

std::vector<vk::attachment::attachment_ptr_t> wall_reply_new::attachments() const
{
    if (has_attachments_) {
        return event::get_attachments(get_event()["attachments"].get_array());
    }
    throw error::access_error(-1, "Attempting accessing empty attachment list");
}

std::ostream& operator<<(std::ostream& ostream, const vk::event::wall_reply_new& reply)
{
    ostream << "wall_reply_new:" << std::endl;

    ostream << std::setw(30) << "id: " << reply.id() << std::endl;
    ostream << std::setw(30) << "from_id: " << reply.from_id() << std::endl;
    ostream << std::setw(30) << "post_id: " << reply.post_id() << std::endl;
    ostream << std::setw(30) << "owner_id: " << reply.owner_id() << std::endl;
    ostream << std::setw(30) << "text: " << reply.text() << std::endl;
    ostream << std::setw(30) << "has_attachments? " << reply.has_attachments() << std::endl;

    if (reply.has_attachments()) {
        for (auto& attachment : reply.attachments()) {
            ostream << std::setw(40) << "attachment: ";
            ostream << attachment->value();
            ostream << std::endl;
        }
    }

    return ostream;
}

}// namespace vk::event
