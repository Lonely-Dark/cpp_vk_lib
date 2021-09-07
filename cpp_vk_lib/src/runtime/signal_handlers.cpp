#include "cpp_vk_lib/runtime/signal_handlers.hpp"

#include "cpp_vk_lib/runtime/stacktrace_dump.hpp"
#include "spdlog/spdlog.h"

#include <csignal>

void runtime::setup_signal_handlers()
{
#if defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
    for (size_t i = 0; i < NSIG; ++i) {
        signal(i, [](int signal) {
            spdlog::critical(
                "{} signal got, showing stacktrace dump...",
                strsignal(signal));
            runtime::stacktrace_dump();
            exit(-1);
        });
    }
#endif
}
