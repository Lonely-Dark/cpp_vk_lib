#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/keyboard/layout.hpp"

int main(int argc, char* argv[])
{
    setup_cpp_vk_lib_environment();

    SECTION(location_button)
    {
        vk::keyboard::layout l;
        l.add_row(
            std::make_shared<vk::keyboard::button::location>()
        );
        l.serialize();
        TEST_CASE(l.get() == R"({"buttons":[[{"action":{"type":"location","payload":"{\"button\":\"1\"}"}}]]})");
    }
    SECTION(open_app)
    {
        vk::keyboard::layout l(vk::keyboard::flag::one_time);
        l.add_row(
            std::make_shared<vk::keyboard::button::open_app>(100, 200, "hash", "label")
        );
        l.serialize();

        TEST_CASE(l.get() == R"({"one_time":true,"buttons":[[{"action":{"type":"open_app","app_id":100,"owner_id":200,"hash":"hash","label":"label"}}]]})");
    }
    SECTION(text)
    {
        vk::keyboard::layout l(vk::keyboard::flag::one_time);
        l.add_row(
            std::make_shared<vk::keyboard::button::text>(vk::keyboard::color::blue, "1"),
            std::make_shared<vk::keyboard::button::text>(vk::keyboard::color::blue, "2")
        );
        l.serialize();
        TEST_CASE(l.get() == R"({"one_time":true,"buttons":[[{"action":{"type":"text","payload":"{\"button\":\"1\"}","label":"1"},"color":"primary"},{"action":{"type":"text","payload":"{\"button\":\"1\"}","label":"2"},"color":"primary"}]]})");
    }
    SECTION(vk_pay)
    {
        vk::keyboard::layout l(vk::keyboard::flag::one_time);
        l.add_row(
            std::make_shared<vk::keyboard::button::vk_pay>("hash")
        );
        l.serialize();
        TEST_CASE(l.get() == R"({"one_time":true,"buttons":[[{"action":{"type":"vkpay","hash":"hash"}}]]})");
    }

    return EXIT_SUCCESS;
}