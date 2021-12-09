#include "cpp_vk_lib/vk/keyboard/layout.hpp"

#include "cpp_vk_lib/runtime/string_utils/implementation/join.hpp"
#include "cpp_vk_lib/vk/error/exception.hpp"

#include <algorithm>

namespace vk::keyboard {

layout::layout(flag flags)
    : serialized_()
    , buttons_()
    , flags_(flags)
{}

void layout::serialize()
{
    serialized_.clear();
    serialized_.reserve(250);
    serialized_.push_back('{');
    if (has_flag(flag::in_line)) {
        serialized_.append("\"inline\":true,");
    }
    if (has_flag(flag::one_time)) {
        serialized_.append("\"one_time\":true,");
    }
    serialized_.append("\"buttons\":[");
    std::vector<std::string> serialized_rows;
    for (const std::vector<std::shared_ptr<button::base>>& row : buttons_) {
        std::vector<std::string> serialized_row;
        for (const std::shared_ptr<button::base>& button : row) {
            serialized_row.push_back(button->serialize());
        }
        serialized_rows.push_back('[' + runtime::string_utils::join(serialized_row, ',') + ']');
    }
    serialized_ += runtime::string_utils::join(serialized_rows, ',');
    serialized_ += "]}";
    serialized_.shrink_to_fit();
}

const std::string& layout::get() const noexcept
{
    return serialized_;
}

bool layout::has_flag(keyboard::flag flag) const noexcept
{
    return (flags_ & flag) > 0;
}

}// namespace vk::keyboard
