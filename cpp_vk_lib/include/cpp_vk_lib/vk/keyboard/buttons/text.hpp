#ifndef VK_KEYBOARD_BUTTONS_TEXT_HPP
#define VK_KEYBOARD_BUTTONS_TEXT_HPP

#include "cpp_vk_lib/vk/keyboard/buttons/base.hpp"
#include "cpp_vk_lib/vk/keyboard/colors.hpp"
#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"

namespace vk::keyboard::button {

class text : public button::base
{
public:
    text(color selected_color, std::string_view payload_data, std::string_view text_message)
        : selected_color_(selected_color)
        , payload_data_(payload_data)
        , text_message_(text_message){}

    std::string serialize() const override
    {
        const char* color = keyboard::get_color(selected_color_);
        return fmt::format(
            R"__({{"action":{{"type":"text","payload":"{}","label":"{}"}},"color":"{}"}})__",
            payload_data_,
            text_message_,
            color);
    }

private:
    color selected_color_ = color::none;
    std::string payload_data_;
    std::string text_message_;
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_TEXT_HPP
