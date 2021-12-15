#ifndef RUNTIME_SIGNAL_HANDLERS_HPP
#define RUNTIME_SIGNAL_HANDLERS_HPP

namespace runtime {
/*!
 * Show stacktrace dump on every incoming signal.
 *
 * This functions just shows call trace on SIGCHLD, SIGCONT, SIGUSR1, SIGUSR2 and SIGURG
 * signals since they're don't critical. E.g SIGCHLD is sent from fork() syscall at thread
 * exit.
 */
void setup_signal_handlers();

}// namespace runtime

#endif// RUNTIME_SIGNAL_HANDLERS_HPP
