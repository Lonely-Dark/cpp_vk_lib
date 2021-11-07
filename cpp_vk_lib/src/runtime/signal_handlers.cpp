#include "cpp_vk_lib/runtime/signal_handlers.hpp"

#include "cpp_vk_lib/runtime/stacktrace_dump.hpp"
#include "spdlog/spdlog.h"

#include <csignal>

void runtime::setup_signal_handlers()
{
    std::vector<int> critical_signals;
    std::vector<int> info_signals;
#if defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
    for (int sig :
         {SIGABRT, SIGALRM, SIGBUS,  SIGFPE,  SIGHUP,  SIGILL,  SIGINT, SIGKILL, SIGPIPE,   SIGQUIT, SIGSEGV,
          SIGTERM, SIGSTOP, SIGTSTP, SIGTTIN, SIGTTOU, SIGPROF, SIGSYS, SIGTRAP, SIGVTALRM, SIGXCPU, SIGXFSZ}) {
        critical_signals.push_back(sig);
    }
#endif

#if defined(__linux__) && !defined(__APPLE__) && !defined(__FreeBSD__)
    critical_signals.push_back(SIGPOLL);
#endif

#if defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
    for (int sig : {SIGCHLD, SIGCONT, SIGUSR1, SIGUSR2, SIGURG}) { info_signals.push_back(sig); }
    for (int sig : critical_signals) {
        signal(sig, [](int signal) {
            spdlog::critical("{} signal got, showing stacktrace dump and exit...", strsignal(signal));
            runtime::stacktrace_dump();
            exit(-1);
        });
    }
    for (int sig : info_signals) {
        signal(sig, [](int signal) {
            spdlog::critical("{} signal got, showing stacktrace dump...", strsignal(signal));
            runtime::stacktrace_dump();
        });
    }
#endif
}
