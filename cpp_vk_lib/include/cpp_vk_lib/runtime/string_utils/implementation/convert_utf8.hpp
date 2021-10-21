#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_UTF8_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_UTF8_HPP

#include <codecvt>
#include <locale>

namespace runtime::string_utils::implementation {

inline std::string utf8_create(
    std::string_view data,
    const std::function<wchar_t(wchar_t, const std::locale&)>& convert)
{
    static const std::locale utf_8_locale("en_US.UTF-8");
    std::wstring wide_string =
        std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(data.data());
    for (auto& c : wide_string) {
        c = convert(c, utf_8_locale);
    }
    return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wide_string);
}

}// namespace runtime::string_utils::implementation

namespace runtime::string_utils {

inline std::string utf8_to_upper(std::string_view data)
{
    return implementation::utf8_create(data, [](wchar_t c, const std::locale& locale) {
        return std::toupper(c, locale);
    });
}

inline std::string utf8_to_lower(std::string_view data)
{
    return implementation::utf8_create(data, [](wchar_t c, const std::locale& locale) {
        return std::tolower(c, locale);
    });
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_CONVERT_UTF8_HPP
