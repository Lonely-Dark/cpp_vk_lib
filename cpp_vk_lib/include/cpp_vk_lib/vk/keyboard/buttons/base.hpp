#ifndef VK_KEYBOARD_BUTTONS_BASE_HPP
#define VK_KEYBOARD_BUTTONS_BASE_HPP

#include <string>

namespace vk::keyboard::button {

/*! Buttons are implemented as inheritance hierarchy just to simplify code,
 *  avoiding a lot of boilerplate code with std::variant's or std::any's.
 */
class base
{
public:
    virtual ~base() = default;

    virtual std::string serialize() const = 0;
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_BASE_HPP
