#ifndef RUNTIME_UNMOVABLE_HPP
#define RUNTIME_UNMOVABLE_HPP

namespace runtime {

struct unmovable
{
    unmovable()            = default;
    unmovable(unmovable&&) = delete;
    unmovable& operator=(unmovable&&) = delete;
};

}// namespace runtime

#endif// RUNTIME_UNMOVABLE_HPP
