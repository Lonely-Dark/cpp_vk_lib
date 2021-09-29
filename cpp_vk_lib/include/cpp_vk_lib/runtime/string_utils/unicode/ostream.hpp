#ifndef RUNTIME_STRING_UTILS_UNICODE_OSTREAM_HPP
#define RUNTIME_STRING_UTILS_UNICODE_OSTREAM_HPP

#include <codecvt>
#include <locale>

namespace runtime::string_utils::ostream_operators {

inline std::ostream& operator<<(std::ostream& os, std::wstring_view s)
{
    return os << std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().to_bytes(s.data());
}

inline std::ostream& operator<<(std::ostream& os, std::u16string_view s)
{
    return os << std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t>().to_bytes(s.data());
}

inline std::ostream& operator<<(std::ostream& os, std::u32string_view s)
{
    return os << std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().to_bytes(s.data());
}

}// namespace runtime::string_utils::ostream_operators

#endif// RUNTIME_STRING_UTILS_UNICODE_OSTREAM_HPP
