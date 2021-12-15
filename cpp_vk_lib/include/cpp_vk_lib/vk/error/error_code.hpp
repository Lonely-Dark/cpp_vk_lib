#ifndef VK_ERROR_ERROR_CODE_HPP
#define VK_ERROR_ERROR_CODE_HPP

#include <string>

namespace vk {
/*!
 * Error code that can be empty or with error represented as string.
 */
class error_code
{
public:
    /*! Create empty wrapper. */
    error_code() noexcept;
    /*! Create with error. */
    error_code(std::string_view message);
    error_code(const error_code&) = default;
    error_code(error_code&&) noexcept;

    error_code& operator=(const error_code&) = delete;
    error_code& operator=(error_code&&) = delete;

    /*! If any error message already exists, it will be overridden. */
    void assign(std::string_view error);
    /*! Clear error code */
    void clear();
    const std::string& message() const noexcept;
    /*! Check if error present */
    operator bool() const noexcept;

private:
    std::string message_;
    bool has_error_;
};

}// namespace vk

#endif// VK_ERROR_ERROR_CODE_HPP
