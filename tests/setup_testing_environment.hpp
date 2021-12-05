#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/runtime/signal_handlers.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"

#include <iostream>

#define TEST_CASE(stmt) if (!(stmt)) { return EXIT_FAILURE; }

#define SECTION(string)

void setup_cpp_vk_lib_environment()
{
    constexpr char sample_config[] = R"__(
        {
          "api": {
            "access_token": "7483c78eba756697ae4727e5d255d7782226d8bf4dc1a87924897e23278fddd46512c759c960a6468e4cf",
            "user_token": "not used"
          },
          "oauth": {
            "login": "not used",
            "password": "not used"
          },
          "environment": {
            "num_workers": 8,
            "log_path": "logs.txt"
          }
        }
    )__";

    vk::config::load_string(sample_config);
    runtime::setup_signal_handlers();
    runtime::setup_logger(spdlog::level::level_enum::info);
}