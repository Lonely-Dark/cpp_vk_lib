#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP

#include <codecvt>
#include <functional>
#include <locale>
#include <numeric>

namespace runtime::string_utils::implementation {

template <typename Converter>
std::string ascii_convert(std::string_view data, Converter&& convert)
{
    std::string output;
    output.reserve(data.size());

    for (char c : data) {
        output += static_cast<char>(convert(c));
    }

    return output;
}

}// namespace runtime::string_utils::implementation

namespace runtime::string_utils {

/*! Convert ASCII-string to lower case.
 *
 * \note for strings converted to UTF-8 and other encodings behaviour is unspecified.
 */
inline std::string ascii_to_lower(std::string_view data)
{
    return implementation::ascii_convert(data, tolower);
}

/*! Convert ASCII-string to upper case.
 *
 * \note for strings converted to UTF-8 and other encodings behaviour is unspecified.
 */
inline std::string ascii_to_upper(std::string_view data)
{
    return implementation::ascii_convert(data, toupper);
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_ASCII_HPP
