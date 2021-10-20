#ifndef RUNTIME_RESULT_HPP
#define RUNTIME_RESULT_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

namespace runtime {

template <typename First, typename Second>
struct result : protected std::pair<First, Second>
{
    result() noexcept
        : result(First{}, Second{})
    {
        static_assert(std::is_convertible_v<Second, bool>);
        static_assert(std::is_copy_constructible_v<First> || std::is_move_constructible_v<First>);
    }

    result(First&& value) noexcept
        : result(std::forward<First>(value), Second{})
    {}

    result(First&& value, Second&& error) noexcept
        : std::pair<First, Second>(std::forward<First>(value), std::forward<Second>(error))
    {}

    void set_error(Second&& error) noexcept
    {
        this->second = std::forward<Second>(error);
    }

    void tie(First& value, Second& error) noexcept
    {
        error = this->second;
        if (!error) {
            value = std::forward<result<First, Second>>(*this).first;
        }
    }

    const Second& error() const noexcept
    {
        return this->second;
    }
    /*!
     * We cannot retrieve value if we have error.
     */
    const First& value() const
    {
        if (error()) {
            throw std::runtime_error("failed to get value(): error exists");
        }
        return this->first;
    }
};

}// namespace runtime

#endif// RUNTIME_RESULT_HPP
