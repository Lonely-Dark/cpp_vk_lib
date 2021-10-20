#ifndef VK_KEYBOARD_BUTTONS_TEXT_HPP
#define VK_KEYBOARD_BUTTONS_TEXT_HPP

#include "cpp_vk_lib/vk/keyboard/colors.hpp"
#include "fmt/format.h"
#include "fmt/ostream.h"

namespace vk::keyboard::button {

class text
{
public:
    text(color selected_color, std::string_view payload_data)
        : selected_color_(selected_color)
        , payload_data_(payload_data)
    {}

    std::string serialize() const
    {
        const char* color = keyboard::get_color(selected_color_);
        return fmt::format(
            R"__({{"action":{{"type":"text","payload":"{{\"button\":\"1\"}}","label":"{}"}},"color":"{}"}})__",
            payload_data_,
            color);
    }

private:
    color selected_color_ = color::none;
    std::string payload_data_;
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_TEXT_HPP
