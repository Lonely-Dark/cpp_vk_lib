#ifndef VK_KEYBOARD_FLAGS_HPP
#define VK_KEYBOARD_FLAGS_HPP

namespace vk::keyboard {

enum struct flag
{
    none     = 1 << 0,
    in_line  = 1 << 1,
    one_time = 1 << 2
};

constexpr unsigned char operator&(flag lhs, flag rhs) noexcept
{
    return static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs);
}

}// namespace vk::keyboard

#endif// VK_KEYBOARD_FLAGS_HPP
