#include "cpp_vk_lib/vk/error/exception.hpp"

#include "spdlog/fmt/fmt.h"

namespace vk::error {

common_exception::common_exception(std::string_view what_arg)
    : error_(what_arg.data())
{}

const char* common_exception::what() const noexcept
{
    return error_.what();
}

std::string common_exception::create(int32_t id, const char* error_name, const char* arg)
{
    return fmt::format("[vk.exception.{}.{}]: {}", error_name, id, arg);
}

upload_error::upload_error(int32_t id, const char* what_arg)
    : common_exception(create(id, "upload_error", what_arg))
{}

access_error::access_error(int32_t id, const char* what_arg)
    : common_exception(create(id, "access_error", what_arg))
{}

invalid_parameter_error::invalid_parameter_error(int32_t id, const char* what_arg)
    : common_exception(create(id, "invalid_parameter_error", what_arg))
{}

runtime_error::runtime_error(int32_t id, const char* what_arg)
    : common_exception(create(id, "runtime_error", what_arg))
{}

}// namespace vk::error
