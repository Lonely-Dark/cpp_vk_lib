#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_SPLIT_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_SPLIT_HPP

#include <string_view>
#include <vector>

namespace runtime::string_utils {

template <typename Delimiter>
std::vector<std::string_view> split(std::string_view data, Delimiter&& delim)
{
    std::vector<std::string_view> splitted;
    splitted.reserve(data.length() / 2);
    size_t pos = 0;
    while (pos != std::string_view::npos) {
        pos = data.find_first_not_of(delim);
        if (pos == std::string_view::npos) { return splitted; }
        data = data.substr(pos);
        pos = data.find_first_of(delim);
        splitted.emplace_back(data.substr(0, pos));
        data = data.substr(pos == std::string_view::npos ? 0 : pos);
    }
    splitted.shrink_to_fit();
    return splitted;
}

inline std::vector<std::string_view> whitespace_split(std::string_view data)
{
    return split(data, " \f\n\r\t\v");
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_SPLIT_HPP
