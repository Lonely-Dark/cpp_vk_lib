#ifndef VK_KEYBOARD_COLORS_HPP
#define VK_KEYBOARD_COLORS_HPP

namespace vk::keyboard::colors {

static inline constexpr char red[] = "negative";
static inline constexpr char green[] = "positive";
static inline constexpr char blue[] = "primary";
static inline constexpr char white[] = "secondary";

}// namespace vk::keyboard::colors

namespace vk::keyboard {

enum class color : unsigned char
{
    none = (1 << 0),
    red = (1 << 1),
    green = (1 << 2),
    blue = (1 << 3),
    white = (1 << 4)
};

constexpr const char* get_color(color c) noexcept
{
    switch (c) {
        case color::red: { return colors::red; }
        case color::green: { return colors::green; }
        case color::blue: { return colors::blue; }
        default: { return colors::white; }
    }
}

}// namespace vk::keyboard

#endif// VK_KEYBOARD_COLORS_HPP
