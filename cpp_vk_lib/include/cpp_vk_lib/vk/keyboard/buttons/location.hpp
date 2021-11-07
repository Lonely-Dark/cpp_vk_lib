#ifndef VK_KEYBOARD_BUTTONS_LOCATION_HPP
#define VK_KEYBOARD_BUTTONS_LOCATION_HPP

#include "spdlog/fmt/fmt.h"

namespace vk::keyboard::button {

class location
{
public:
    std::string serialize() const noexcept
    {
        /// Escape '{' and '}' like other button payloads.
        return fmt::format(R"__({{"action":{{"type":"location","payload":"{{\"button\":\"1\"}}"}}}})__");
    }
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_LOCATION_HPP
