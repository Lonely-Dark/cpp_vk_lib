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
        if (Dl_info info; dladdr(address, &info) && info.dli_sname) { symbol = info.dli_sname; }
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
    void* buffer[/*max_records=*/25];
    int addresses_got = backtrace(buffer, sizeof(buffer) / sizeof(void*));
    if (addresses_got == 0) {
        spdlog::critical("  empty stack trace, exiting...");
        return;
    }
    char** stack_dump = backtrace_symbols(buffer, addresses_got);
    char* demangled_name = static_cast<char*>(malloc(256));
    for (int i = 0; i < addresses_got; ++i) {
      char *mangled_name { nullptr },
           *begin_offset { nullptr },
           *end_offset { nullptr };
      for (char* symbol = stack_dump[i]; *symbol; ++symbol) {
          int need_exit = 0;
          switch (*symbol) {
              case '(': { mangled_name = symbol; break; }
              case '+': { begin_offset = symbol; break; }
              case ')': { end_offset = symbol; need_exit = 1; break; }
              default: { break; }
          }
          if (need_exit == 1) { break; }
      }
      if (mangled_name && begin_offset && end_offset && mangled_name < begin_offset) {
          *(mangled_name++) = '\0';
          *(begin_offset++) = '\0';
          *end_offset = '\0';
          int status = 0;
          size_t max_length = 256;
          char* demangled = abi::__cxa_demangle(mangled_name, demangled_name, &max_length, &status);
          if (status == 0) {
              demangled_name = demangled;
              spdlog::critical("  {}: {}+{}", stack_dump[i], demangled_name, begin_offset);
          } else {
              spdlog::critical("  {}: {}()+{}", stack_dump[i], mangled_name, begin_offset);
          }
      } else {
          spdlog::critical("  {}", stack_dump[i]);
      }
    }
    free(demangled_name);
    free(stack_dump);
    // clang-format off
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
