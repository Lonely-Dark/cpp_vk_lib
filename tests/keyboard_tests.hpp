#include "cpp_vk_lib/vk/keyboard/layout.hpp"

#include <gtest/gtest.h>

namespace keyboard = vk::keyboard;

TEST(keyboard, location_button)
{
    keyboard::layout l;
    l.add_row({keyboard::button::location()});
    l.serialize();

    ASSERT_EQ(l.get(), R"({"buttons":[[{"action":{"type":"location","payload":"{\"button\":\"1\"}"}}]]})");
}

TEST(keyboard, open_app_layout)
{
    keyboard::layout l(keyboard::flag::one_time);
    l.add_row({keyboard::button::open_app(100, 200, "hash", "label")});
    l.serialize();

    ASSERT_EQ(
        l.get(),
        R"({"one_time":true,"buttons":[[{"action":{"type":"open_app","app_id":100,"owner_id":200,"hash":"hash","label":"label"}}]]})");
}

TEST(keyboard, text_layout)
{
    keyboard::layout l(keyboard::flag::one_time);
    l.add_row({keyboard::button::text(keyboard::color::blue, "1"), keyboard::button::text(keyboard::color::blue, "2")});
    l.serialize();

    ASSERT_EQ(
        l.get(),
        R"({"one_time":true,"buttons":[[{"action":{"type":"text","payload":"{\"button\":\"1\"}","label":"1"},"color":"primary"},{"action":{"type":"text","payload":"{\"button\":\"1\"}","label":"2"},"color":"primary"}]]})");
}

TEST(keyboard, vk_pay_button)
{
    keyboard::layout l(keyboard::flag::one_time);
    l.add_row({keyboard::button::vk_pay("hash")});
    l.serialize();

    ASSERT_EQ(l.get(), R"({"one_time":true,"buttons":[[{"action":{"type":"vkpay","hash":"hash"}}]]})");
}

TEST(keyboard, bad_cast)
{
    keyboard::layout l(keyboard::flag::one_time);
    l.add_row({1, 2, 3});
    ASSERT_ANY_THROW(l.serialize());///< Non-button types provided
}
