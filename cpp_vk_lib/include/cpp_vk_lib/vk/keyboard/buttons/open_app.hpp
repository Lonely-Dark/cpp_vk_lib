#ifndef VK_KEYBOARD_BUTTONS_OPEN_APP_HPP
#define VK_KEYBOARD_BUTTONS_OPEN_APP_HPP

#include "cpp_vk_lib/vk/keyboard/buttons/base.hpp"
#include "spdlog/fmt/fmt.h"

namespace vk::keyboard::button {

class open_app : public button::base
{
public:
    open_app(int64_t app_id, int64_t owner_id, std::string_view hash, std::string_view label)
        : app_id_(app_id)
        , owner_id_(owner_id)
        , hash_(hash)
        , label_(label)
    {}

    std::string serialize() const override
    {
        return fmt::format(
            R"__({{"action":{{"type":"open_app","app_id":{},"owner_id":{},"hash":"{}","label":"{}"}}}})__",
            app_id_,
            owner_id_,
            hash_,
            label_);
    }

private:
    int64_t app_id_;
    int64_t owner_id_;
    std::string hash_;
    std::string label_;
};

}// namespace vk::keyboard::button

#endif// VK_KEYBOARD_BUTTONS_OPEN_APP_HPP
