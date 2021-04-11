#ifndef VK_KEYBOARD_FLAGS_H
#define VK_KEYBOARD_FLAGS_H

namespace vk {
namespace keyboard {

enum class flag : unsigned char
{
  none = (1 << 0),
  in_line = (1 << 1),
  one_time = (1 << 2)
};

constexpr inline unsigned char operator&(flag lhs, flag rhs) noexcept {
  return static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs);
}
}// namespace keyboard
}// namespace vk

#endif// VK_KEYBOARD_FLAGS_H
