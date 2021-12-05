#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/error/exception.hpp"

int main(int argc, char* argv[])
{
    SECTION(access_error_throw)
    {
        try {
            throw vk::error::access_error(224, "text");
        } catch (std::exception& error) {
            TEST_CASE(std::string(error.what()) == "[vk.exception.access_error.224]: text");
        }
    }
    SECTION(upload_error_throw)
    {
        try {
            throw vk::error::upload_error(224, "text");
        } catch (std::exception& error) {
            TEST_CASE(std::string(error.what()) == "[vk.exception.upload_error.224]: text");
        }
    }
    SECTION(invalid_parameter_throw)
    {
        try {
            throw vk::error::invalid_parameter_error(224, "text");
        } catch (std::exception& error) {
            TEST_CASE(std::string(error.what()) == "[vk.exception.invalid_parameter_error.224]: text");
        }
    }
    return EXIT_SUCCESS;
}