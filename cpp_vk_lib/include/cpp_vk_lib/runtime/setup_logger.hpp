#ifndef RUNTIME_SETUP_LOGGER_HPP
#define RUNTIME_SETUP_LOGGER_HPP

#include "spdlog/common.h"

namespace runtime {
/*!
 * Setup file, stdout and stderr sinks.
 *
 * Logs will be written in "log_path" specifined in config.
 */
void setup_logger(spdlog::level::level_enum);

}// namespace runtime

#endif// RUNTIME_SETUP_LOGGER_HPP
