#include "cpp_vk_lib/vk/config/config.hpp"

#include <gtest/gtest.h>

TEST(config, field_access)
{
    vk::config::load_string(R"__(
        {
          "api": {
            "access_token": "access_token",
            "user_token": "user_token"
          },
          "oauth": {
            "login": "login@gmail.com",
            "password": "password"
          },
          "environment": {
            "num_workers": 8,
            "log_path": "/tmp/bot.log"
          }
        }
    )__");

    ASSERT_EQ(vk::config::access_token(), "access_token");
    ASSERT_EQ(vk::config::user_token(), "user_token");

    vk::config::override_user_token("new_token");

    ASSERT_EQ(vk::config::user_token(), "new_token");

    ASSERT_EQ(vk::config::username(), "login@gmail.com");
    ASSERT_EQ(vk::config::password(), "password");

    ASSERT_EQ(vk::config::num_workers(), 8);

    ASSERT_EQ(vk::config::log_path(), "/tmp/bot.log");
}
