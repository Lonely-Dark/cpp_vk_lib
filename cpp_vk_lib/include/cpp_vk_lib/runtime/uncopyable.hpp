#ifndef RUNTIME_UNCOPYABLE_HPP
#define RUNTIME_UNCOPYABLE_HPP

namespace runtime {

struct uncopyable
{
    uncopyable()                  = default;
    uncopyable(const uncopyable&) = delete;
    uncopyable& operator=(const uncopyable&) = delete;
};

}// namespace runtime

#endif// RUNTIME_UNCOPYABLE_HPP
