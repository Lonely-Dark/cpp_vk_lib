#include "cpp_vk_lib/runtime/stacktrace_dump.hpp"

#include "spdlog/spdlog.h"

#if defined(__ANDROID__)
#    include <dlfcn.h>
#    include <iomanip>
#    include <sstream>
#    include <unwind.h>
#elif defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
#    include <cxxabi.h>
#    include <execinfo.h>
#endif

#if defined(__ANDROID__)
struct backtrace_state
{
    void** current;
    void** end;
};

static _Unwind_Reason_Code unwind_callback(struct _Unwind_Context* context, void* arg)
{
    backtrace_state* state = static_cast<backtrace_state*>(arg);
    if (uintptr_t pc = _Unwind_GetIP(context)) {
        if (state->current == state->end) {
            return _URC_END_OF_STACK;
        } else {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
    }
    return _URC_NO_REASON;
}

static size_t capture_stacktrace(void** buffer, size_t max)
{
    backtrace_state state{buffer, buffer + max};
    _Unwind_Backtrace(unwind_callback, &state);
    return state.current - buffer;
}

static void android_stacktrace_dump_implementation()
{
    const size_t max_stacktrace_records = 25;
    void* buffer[max_stacktrace_records];
    std::ostringstream ostream;

    const size_t stacktrace_size = capture_stacktrace(buffer, max_stacktrace_records);
    for (size_t i = 0; i < stacktrace_size; ++i) {
        const void* address = buffer[i];
        const char* symbol  = "";
        if (Dl_info info; dladdr(address, &info) && info.dli_sname) {
            symbol = info.dli_sname;
        }
        int status           = 0;
        size_t max_length    = 256;
        char* demangled_name = static_cast<char*>(malloc(/*__size=*/256));
        char* demangled      = abi::__cxa_demangle(symbol, demangled_name, &max_length, &status);
        ostream << "  " << std::setw(2) << ": " << address << " " << demangled;
        << std::endl;
        free(demangled_name);
    }
    spdlog::critical("{}", ostream.str());
}
#elif defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
static void unix_stacktrace_dump_implementation()
{
    // clang-format off
    void* strace_buffer[/*max_records=*/50];
    int strace_size = backtrace(strace_buffer, sizeof(strace_buffer) / sizeof(void*));
    if (strace_size == 0) {
        spdlog::critical("  empty stack trace, exiting...");
        return;
    }
    char** strace = backtrace_symbols(strace_buffer, strace_size);
    char* demangle_buffer = static_cast<char*>(malloc(256));

    for (int i = 0; i < strace_size; ++i) {
        char* begin_mangled_name = nullptr;
        char*   end_mangled_name = nullptr;
        char*             offset = nullptr;
        char*       strace_frame = *(strace + i);

        while (strace_frame) {
            switch (*strace_frame) {
                case '+': { offset = strace_frame; break; }
                case '(': { begin_mangled_name = strace_frame; break; }
                case ')': { end_mangled_name = strace_frame; break; }
            }
            if (end_mangled_name) {
                break;
            }
            ++strace_frame;
        }

        if (!begin_mangled_name || !end_mangled_name || !offset) {
            continue;
        }

        *(begin_mangled_name++) = '\0';
        *(  end_mangled_name++) = '\0';
        *(            offset++) = '\0';

        int demangle_status = 0;
        size_t frame_size = 256;
        demangle_buffer = abi::__cxa_demangle(begin_mangled_name, demangle_buffer, &frame_size, &demangle_status);
        if (demangle_status == 0) {
            spdlog::critical("  {}: {}+{}", strace[i], demangle_buffer, offset);
        } else {
            spdlog::critical("  {}: {}()+{}", strace[i], begin_mangled_name, offset);
        }
    }

    free(demangle_buffer);
    free(strace);
    // clang-format on
}
#else
static void unknown_platform_stacktrace_dump_implementation()
{
    spdlog::critical("sorry, stacktrace isn't supported for your platform yet");
}
#endif

void runtime::stacktrace_dump()
{
#if defined(__ANDROID__)
    android_stacktrace_dump_implementation();
#elif defined(__FreeBSD__) || defined(__linux__) || defined(__APPLE__)
    unix_stacktrace_dump_implementation();
#else
    unknown_platform_stacktrace_dump_implementation();
#endif
}
