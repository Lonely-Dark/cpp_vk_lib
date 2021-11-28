#ifndef RUNTIME_UNCOPYABLE_HPP
#define RUNTIME_UNCOPYABLE_HPP

namespace runtime {
/*!
 * \brief Should be used by inheritance to prevent object copying and
 * boilerplate deleting copy constructors and assignment operators.
 * \code
 * class my_singleton_object : public uncopyable {};
 * \endcode
 */
// clang-format off
struct uncopyable
{
    uncopyable() = default;
    uncopyable(const uncopyable&) = delete;
    uncopyable& operator=(const uncopyable&) = delete;
};
// clang-format on
}// namespace runtime

#endif// RUNTIME_UNCOPYABLE_HPP
