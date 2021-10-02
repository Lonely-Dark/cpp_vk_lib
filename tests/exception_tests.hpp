#include "cpp_vk_lib/vk/error/exception.hpp"

#include <gtest/gtest.h>

TEST(exception, access_error)
{
    const size_t id = 224;

    try {
        throw vk::error::access_error(id, "text");
    } catch (std::exception& error) {
        ASSERT_EQ(std::string(error.what()), "[vk.exception.access_error.224]: text");
    }
}

TEST(exception, upload_error)
{
    const size_t id = 224;

    try {
        throw vk::error::upload_error(id, "text");
    } catch (std::exception& error) {
        ASSERT_EQ(std::string(error.what()), "[vk.exception.upload_error.224]: text");
    }
}

TEST(exception, invalid_parameter_error)
{
    const size_t id = 224;

    try {
        throw vk::error::invalid_parameter_error(id, "text");
    } catch (std::exception& error) {
        ASSERT_EQ(std::string(error.what()), "[vk.exception.invalid_parameter_error.224]: text");
    }
}
