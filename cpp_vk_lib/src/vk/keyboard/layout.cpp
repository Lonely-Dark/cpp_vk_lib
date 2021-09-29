#include "cpp_vk_lib/vk/keyboard/layout.hpp"

#include "cpp_vk_lib/runtime/result.hpp"
#include "cpp_vk_lib/vk/error/exception.hpp"
#include "spdlog/spdlog.h"

#include <algorithm>

namespace vk::keyboard {

layout::layout(flag flags)
    : serialized_()
    , buttons_()
    , flags_(flags)
{}

void layout::add_row(std::vector<std::any>&& row)
{
    buttons_.push_back(std::move(row));
}

template <typename T>
static VK_REALLY_INLINE runtime::result<std::string, int>
    create_impl(const std::any& button) noexcept
{
    try {
        return std::any_cast<T>(button).serialize();
    } catch (std::bad_any_cast&) {
        return runtime::result(std::string(), -1);
    }
}

static std::string create_button(const std::any& button)
{
    if (auto result = create_impl<button::text>(button); !result.error()) {
        return result.value();
    }
    if (auto result = create_impl<button::vk_pay>(button); !result.error()) {
        return result.value();
    }
    if (auto result = create_impl<button::open_app>(button); !result.error()) {
        return result.value();
    }
    if (auto result = create_impl<button::location>(button); !result.error()) {
        return result.value();
    }
    throw exception::runtime_error(-1, "cannot create button: bad cast");
}

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
    for (const auto& row : buttons_) {
        std::vector<std::string> serialized_buttons;
        std::transform(
            row.begin(),
            row.end(),
            std::back_inserter(serialized_buttons),
            [](const std::any& button) {
                if (spdlog::get_level() == SPDLOG_LEVEL_TRACE) {
                    std::string payload_data = create_button(button);
                    spdlog::trace("create button: {}", payload_data);
                    return payload_data;
                } else {
                    return create_button(button);
                }
            });
        serialized_rows.push_back(
            '[' + runtime::string_utils::join<std::string>(serialized_buttons, ',') + ']');
    }
    serialized_ += runtime::string_utils::join<std::string>(serialized_rows, ',');
    serialized_.append("]}");
    serialized_.shrink_to_fit();
}

const std::string& layout::get() const noexcept
{
    return serialized_;
}

bool layout::has_flag(flag flag) const noexcept
{
    return (flags_ & flag) > 0;
}

}// namespace vk::keyboard
