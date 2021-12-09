#ifndef VK_KEYBOARD_BUTTONS_LOCATION_HPP
#define VK_KEYBOARD_BUTTONS_LOCATION_HPP

#include "cpp_vk_lib/vk/keyboard/buttons/base.hpp"

namespace vk::keyboard::button {

class location : public button::base
{
public:
    std::string serialize() const override
    {
        /// Escape '{' and '}' like other button payloads.
        return R"__({"action":{"type":"location","payload":"{\"button\":\"1\"}"}})__";
    }
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_LOCATION_HPP
