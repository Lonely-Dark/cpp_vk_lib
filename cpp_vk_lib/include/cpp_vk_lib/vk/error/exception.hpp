#ifndef VK_ERROR_EXCEPTION_HPP
#define VK_ERROR_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <string_view>

namespace vk::error {

/*! General exception of VK method classes. */
class common_exception : public std::exception
{
public:
    const char* what() const noexcept override;

protected:
    explicit common_exception(std::string_view what_arg);
    /*! Return formatted string in form "[vk.exception.id.exception_name]: arg". */
    static std::string create(int32_t id, const char* exception_name, const char* arg);

private:
    std::runtime_error error_;
};

class upload_error : public common_exception
{
public:
    explicit upload_error(int32_t id, const char* what_arg);
};

class access_error : public common_exception
{
public:
    explicit access_error(int32_t id, const char* what_arg);
};

class invalid_parameter_error : public common_exception
{
public:
    explicit invalid_parameter_error(int32_t id, const char* what_arg);
};

class runtime_error : public common_exception
{
public:
    explicit runtime_error(int32_t id, const char* what_arg);
};

/*! This is used to inform the user about the wrong cast, showing template parameters on throw. */
template <typename Base, typename Derived>
class bad_cast_error : public common_exception
{
public:
    bad_cast_error()
        : common_exception(create(0, "bad_cast_error", "Cast error"))
    {}
};

}// namespace vk::error

#endif// VK_ERROR_EXCEPTION_HPP
