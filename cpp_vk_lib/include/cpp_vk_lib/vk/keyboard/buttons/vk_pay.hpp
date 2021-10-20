#ifndef VK_KEYBOARD_BUTTONS_VK_PAY_HPP
#define VK_KEYBOARD_BUTTONS_VK_PAY_HPP

#include "spdlog/fmt/fmt.h"

namespace vk::keyboard::button {

class vk_pay
{
public:
    vk_pay(std::string_view hash)
        : hash_(hash)
    {}

    std::string serialize() const
    {
        return fmt::format(
            R"__({{"action":{{"type":"vkpay","hash":"{}"}}}})__",
            hash_);
    }

private:
    std::string hash_;
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_VK_PAY_HPP
