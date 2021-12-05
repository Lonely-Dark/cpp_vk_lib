#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/config/config.hpp"

int main(int argc, char* argv[])
{
    SECTION(load_config)
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

        TEST_CASE(vk::config::access_token() == "access_token");
        TEST_CASE(vk::config::user_token() == "user_token");

        vk::config::override_user_token("new_token");

        TEST_CASE(vk::config::user_token() == "new_token");
        TEST_CASE(vk::config::username() == "login@gmail.com");
        TEST_CASE(vk::config::password() == "password");
        TEST_CASE(vk::config::num_workers() == 8);
        TEST_CASE(vk::config::log_path() == "/tmp/bot.log");
    }
    return EXIT_SUCCESS;
}
