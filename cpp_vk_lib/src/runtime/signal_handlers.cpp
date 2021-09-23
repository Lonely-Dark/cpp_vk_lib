#include "cpp_vk_lib/runtime/signal_handlers.hpp"

#include "cpp_vk_lib/runtime/stacktrace_dump.hpp"
#include "spdlog/spdlog.h"

#include <csignal>

void runtime::setup_signal_handlers()
{
#if defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
    for (int sig : {SIGABRT, SIGALRM, SIGBUS,    SIGFPE,  SIGHUP,  SIGILL,
                    SIGINT,  SIGKILL, SIGPIPE,   SIGQUIT, SIGSEGV, SIGTERM,
                    SIGSTOP, SIGTSTP, SIGTTIN,   SIGTTOU, SIGPOLL, SIGPROF,
                    SIGSYS,  SIGTRAP, SIGVTALRM, SIGXCPU, SIGXFSZ}) {
        signal(sig, [](int signal) {
            spdlog::critical(
                "{} signal got, showing stacktrace dump and exit...",
                strsignal(signal));
            runtime::stacktrace_dump();
            exit(-1);
        });
    }
    for (int sig : {SIGCHLD, SIGCONT, SIGUSR1, SIGUSR2, SIGURG}) {
        signal(sig, [](int signal) {
            spdlog::critical(
                "{} signal got, showing stacktrace dump...",
                strsignal(signal));
            runtime::stacktrace_dump();
        });
    }
#endif
}
