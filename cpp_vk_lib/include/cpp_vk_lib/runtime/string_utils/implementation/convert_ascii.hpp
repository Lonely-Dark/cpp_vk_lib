#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP

#include <codecvt>
#include <functional>
#include <locale>
#include <numeric>

namespace runtime::string_utils::implementation {

inline std::string ascii_convert(std::string_view data, std::function<int(int)> convert)
{
    // clang-format off
    return std::accumulate(data.begin(), data.end(), std::string(),
        [&convert, length = data.length()](std::string& string, char c) {
            if (string.empty()) {
                string.reserve(length);
            }
            return string += static_cast<char>(convert(c));
        });
    // clang-format on
}

}// namespace runtime::string_utils::implementation

namespace runtime::string_utils {

inline std::string ascii_to_lower(std::string_view data)
{
    return implementation::ascii_convert(data, tolower);
}

inline std::string ascii_to_upper(std::string_view data)
{
    return implementation::ascii_convert(data, toupper);
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP
