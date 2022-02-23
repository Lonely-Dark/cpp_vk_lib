#ifndef RUNTIME_RESULT_HPP
#define RUNTIME_RESULT_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

namespace runtime {

/*! Optionally error-storing wrapper around Value type. */
template <typename Value, typename Error>
struct result : protected std::pair<Value, Error>
{
    static_assert(std::is_convertible_v<Error, bool>);
    static_assert(std::is_copy_constructible_v<Value> && std::is_move_constructible_v<Value>);

    /*! Set both value and error to default values. */
    result() noexcept
        : result(Value{}, Error{}) {}

    /*!
     * Set value and set error to false.
     *
     * \note Not marked as explicit to allow simply deduction to Value type.
     */
    result(Value&& value)
        : result(std::forward<Value>(value), Error{false}) {}

    /*! Set both value and error. */
    result(Value&& value, Error&& error) noexcept
        : std::pair<Value, Error>(std::forward<Value>(value), std::forward<Error>(error)) {}

    void set_error(Error&& error) noexcept
    {
        this->second = std::forward<Error>(error);
    }

    /*! Reset error, then set value, if newly assigned error isn't present. */
    void tie(Value& value, Error& error) noexcept
    {
        error = this->second;
        if (!error) {
            value = std::forward<result<Value, Error>>(*this).first;
        }
    }

    const Error& error() const noexcept
    {
        return this->second;
    }

    /*!
     * Return value if error isn't present, throw otherwise.
     *
     * \throws std::runtime_error
     */
    const Value& value() const
    {
        if (error()) {
            throw std::runtime_error("Failed to get value: error exists");
        }
        return this->first;
    }
};

}// namespace runtime

#endif// RUNTIME_RESULT_HPP
