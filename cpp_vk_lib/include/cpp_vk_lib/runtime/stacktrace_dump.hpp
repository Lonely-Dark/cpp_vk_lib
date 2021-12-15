#ifndef RUNTIME_STACKTRACE_DUMP_HPP
#define RUNTIME_STACKTRACE_DUMP_HPP

namespace runtime {
/*!
 * Create stacktrace dump and put it to screen on every incoming signal.
 *
 * \note At this moment, stacktrace dumps for Unix and Android only are implemented.
 */
void stacktrace_dump();

}// namespace runtime

#endif// RUNTIME_STACKTRACE_DUMP_HPP
