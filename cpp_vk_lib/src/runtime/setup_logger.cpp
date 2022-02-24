#include "cpp_vk_lib/runtime/setup_logger.hpp"

#include "cpp_vk_lib/vk/config/config.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

void runtime::setup_logger(spdlog::level::level_enum level)
{
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always);
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(vk::config::log_path(), 1024 * 1024 * 10, 3);
    std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());
    logger->set_level(level);
    logger->set_pattern("[%b %d %Y %H:%M:%S:%f] (%l): %v");
    spdlog::set_default_logger(logger);
}
