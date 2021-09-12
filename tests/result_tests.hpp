#include "cpp_vk_lib/runtime/result.hpp"

#include <gtest/gtest.h>

TEST(result, success)
{
    runtime::result<std::string, size_t> result("123");
    ASSERT_FALSE(result.error());
}

TEST(result, error)
{
    runtime::result<std::string, size_t> result("123", 1);
    ASSERT_TRUE(result.error());
    ASSERT_EQ(result.error(), 1);
}

TEST(result, set_error)
{
    runtime::result<std::string, size_t> result("123");
    result.set_error(1);
    ASSERT_TRUE(result.error());
    ASSERT_EQ(result.error(), 1);
}

TEST(result, tie)
{
    runtime::result<std::string, size_t> result("123", 0);
    std::string error_body;
    size_t error_code;
    result.tie(error_body, error_code);
    ASSERT_EQ(error_body, "123");
    ASSERT_EQ(error_code, 0);
}

TEST(result, tie_no_error)
{
    runtime::result<std::string, size_t> result("123", 1);
    std::string error_body;
    size_t error_code;
    result.tie(error_body, error_code);
    ASSERT_EQ(error_body, std::string());
    ASSERT_EQ(error_code, 1);
}

TEST(result, template_deduction)
{
    runtime::result auto_filled(100, 200);
    try {
        auto_filled.value();
    } catch (std::runtime_error& e) {}
    ASSERT_EQ(auto_filled.error(), 200);
}
