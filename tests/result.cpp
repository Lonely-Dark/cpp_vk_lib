#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/runtime/result.hpp"

int main(int argc, char* argv[])
{
    SECTION(success)
    {
        runtime::result<std::string, size_t> result("123");
        TEST_CASE(!result.error());
    }
    SECTION(error)
    {
        runtime::result<std::string, size_t> result("123", 1);
        TEST_CASE(result.error());
        TEST_CASE(result.error() == 1);
    }
    SECTION(set_error)
    {
        runtime::result<std::string, size_t> result("123");
        result.set_error(1);
        TEST_CASE(result.error());
        TEST_CASE(result.error() == 1);
    }
    SECTION(tie)
    {
        runtime::result<std::string, size_t> result("123", 0);
        std::string error_body;
        size_t error_code;
        result.tie(error_body, error_code);
        TEST_CASE(error_body == "123");
        TEST_CASE(!error_code);
    }
    SECTION(tie_no_error)
    {
        runtime::result<std::string, size_t> result("123", 1);
        std::string error_body;
        size_t error_code;
        result.tie(error_body, error_code);
        TEST_CASE(error_body == std::string());
        TEST_CASE(error_code == 1);
    }
    SECTION(template_deduction)
    {
        runtime::result auto_filled(100, 200);
        try {
            auto_filled.value();
        } catch (std::runtime_error& e) {}
        TEST_CASE(auto_filled.error() == 200);
    }
    return EXIT_SUCCESS;
}