#ifndef RUNTIME_UNREACHABLE_POINT_HPP
#define RUNTIME_UNREACHABLE_POINT_HPP

#include <cstdlib>

namespace runtime {

[[noreturn]] void unreachable_point() {
    abort();
}

} // namespace runtime

#endif //RUNTIME_UNREACHABLE_POINT_HPP
