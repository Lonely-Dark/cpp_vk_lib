#include "cpp_vk_lib/vk/events/wall_repost.hpp"

#include "spdlog/spdlog.h"

#include <iomanip>
#include <ostream>

namespace vk::event {

wall_repost::wall_repost(int64_t id, int64_t from_id, int64_t owner_id, std::string text)
    : id_(id)
    , from_id_(from_id)
    , owner_id_(owner_id)
    , text_(std::move(text))
    , attachments_()
{
    if (spdlog::get_level() == SPDLOG_LEVEL_TRACE) {
        std::ostringstream ostream;
        ostream << "creating an event ";
        ostream << *this;
        spdlog::trace("{}", ostream.str());
    }
}

void wall_repost::construct_attachments(std::vector<vk::attachment::attachment_ptr_t>&& attachments)
{
    attachments_ = std::move(attachments);
}

bool wall_repost::has_attachments() const noexcept
{
    return !attachments_.empty();
}

int64_t wall_repost::id() const noexcept
{
    return id_;
}

int64_t wall_repost::from_id() const noexcept
{
    return from_id_;
}

int64_t wall_repost::owner_id() const noexcept
{
    return owner_id_;
}

const std::string& wall_repost::text() const noexcept
{
    return text_;
}

const std::vector<vk::attachment::attachment_ptr_t>& wall_repost::attachments() const noexcept
{
    return attachments_;
}

std::ostream& operator<<(std::ostream& ostream, const vk::event::wall_repost& event)
{
    ostream << "wall_repost:" << std::endl;
    ostream << std::setw(30) << "id: " << event.id() << std::endl;
    ostream << std::setw(30) << "from_id: " << event.from_id() << std::endl;
    ostream << std::setw(30) << "owner_id: " << event.owner_id() << std::endl;
    ostream << std::setw(30) << "text: " << event.text() << std::endl;
    if (event.has_attachments()) {
        for (auto& attachment : event.attachments()) {
            ostream << std::setw(30) << "attachment: ";
            ostream << attachment->value();
            ostream << std::endl;
        }
    }
    return ostream;
}

}// namespace vk::event
