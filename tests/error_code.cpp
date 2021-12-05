#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/error/error_code.hpp"

int main(int argc, char* argv[])
{
    SECTION(basic)
    {
        vk::error_code errc;
        TEST_CASE(!static_cast<bool>(errc));
    }
    SECTION(assign)
    {
        vk::error_code errc;
        TEST_CASE(!static_cast<bool>(errc));
        errc.assign("Something went wrong :(");
        TEST_CASE(static_cast<bool>(errc));
    }
    SECTION(assign_and_clear)
    {
        vk::error_code errc;
        TEST_CASE(!static_cast<bool>(errc));
        errc.assign("Something went wrong :(");
        TEST_CASE(static_cast<bool>(errc));
        errc.clear();
        TEST_CASE(!static_cast<bool>(errc));
    }
    return EXIT_SUCCESS;
}