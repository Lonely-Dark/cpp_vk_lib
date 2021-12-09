#ifndef VK_KEYBOARD_BUTTONS_BASE_HPP
#define VK_KEYBOARD_BUTTONS_BASE_HPP

#include <string>

namespace vk::keyboard::button {

class base
{
public:
    virtual std::string serialize() const = 0;
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_BASE_HPP
