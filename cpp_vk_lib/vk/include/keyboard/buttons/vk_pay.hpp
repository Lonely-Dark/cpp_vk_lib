#ifndef VK_KEYBOARD_BUTTONS_VK_PAY_HPP
#define VK_KEYBOARD_BUTTONS_VK_PAY_HPP

#include "runtime/include/string_utils/string_utils.hpp"

namespace vk {
namespace keyboard {
namespace button {

class vk_pay
{
public:
    vk_pay(std::string_view hash)
        : hash_(hash) {}

    std::string serialize() const
    {
        return runtime::string_utils::format(R"__({"action":{"type":"vkpay","hash":"{}"}})__", hash_);
    }

private:
    std::string hash_;
};
}// namespace button
}// namespace keyboard
}// namespace vk

#endif// VK_KEYBOARD_BUTTONS_VK_PAY_HPP
