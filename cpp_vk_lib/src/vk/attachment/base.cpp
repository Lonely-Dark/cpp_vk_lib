#include "cpp_vk_lib/runtime/string_utils/string_utils.hpp"
#include "cpp_vk_lib/vk/attachment/attachment.hpp"

namespace vk::attachment {

base::base(std::string_view type, int32_t owner_id, int32_t id)
    : attachment_type_(type)
    , owner_id_(owner_id)
    , id_(id)
{}

std::string base::value() const
{
    return runtime::string_utils::format("{}{}_{}", attachment_type_, owner_id_, id_);
}

const std::string& base::type() const noexcept
{
    return attachment_type_;
}

}// namespace vk::attachment
