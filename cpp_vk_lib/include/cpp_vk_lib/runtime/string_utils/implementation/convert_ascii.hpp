#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP

#include <codecvt>
#include <locale>
#include <numeric>

namespace runtime::string_utils {

inline std::string ascii_to_lower(std::string_view data)
{
    // clang-format off
    return std::accumulate(data.begin(), data.end(), std::string(),
        [length = data.length()](std::string& string, char c) {
            if (string.empty()) {
                string.reserve(length);
            }
            return string += static_cast<char>(std::tolower(c));
        });
    // clang-format on
}

inline std::string ascii_to_upper(std::string_view data)
{
    // clang-format off
    return std::accumulate( data.begin(), data.end(), std::string(),
        [length = data.length()](std::string& string, char c) {
            if (string.empty()) {
                string.reserve(length);
            }
            return string += static_cast<char>(std::toupper(c));
        });
    // clang-format on
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP
