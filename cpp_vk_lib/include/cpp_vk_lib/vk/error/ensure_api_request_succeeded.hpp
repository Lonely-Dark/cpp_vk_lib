#ifndef VK_ERROR_ENSURE_API_REQUEST_SUCCEEDED_HPP
#define VK_ERROR_ENSURE_API_REQUEST_SUCCEEDED_HPP

#include "cpp_vk_lib/vk/error/exception.hpp"

#include <cstddef>
#include <unordered_map>

namespace simdjson {
namespace dom {
class object;
} // namespace dom
} // namespace simdjson

namespace vk::error {

/*! Parse and check string for VK API errors and do throw if it's present. */
void ensure_api_request_succeeded(std::string_view response);
/*! Check JSON object for VK API errors and do throw if it's present. */
void ensure_api_request_succeeded(const simdjson::dom::object& response);

}// namespace vk::error

#endif// VK_ERROR_ENSURE_API_REQUEST_SUCCEEDED_HPP
