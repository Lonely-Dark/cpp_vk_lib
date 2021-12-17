#ifndef RUNTIME_UNMOVABLE_HPP
#define RUNTIME_UNMOVABLE_HPP

namespace runtime {
/*!
 * \brief Should be used by inheritance to prevent object moving and
 * boilerplate deleting copy constructors and assignment operators.
 * \code
 * class my_singleton_object : public unmovable {};
 * \endcode
 */
struct unmovable
{
    unmovable() = default;
    unmovable(unmovable&&) = delete;
    unmovable& operator=(unmovable&&) = delete;
};
}// namespace runtime

#endif// RUNTIME_UNMOVABLE_HPP
