#ifndef VK_ERROR_TRANSLATE_ERROR_HPP
#define VK_ERROR_TRANSLATE_ERROR_HPP

#include <cstddef>
#include <unordered_map>

namespace vk::error {
/*!
 * \return Detailed VK error description by its code
 */
const char* translate_to_string(size_t error_code);

}// namespace vk::error

#endif// VK_ERROR_TRANSLATE_ERROR_HPP
