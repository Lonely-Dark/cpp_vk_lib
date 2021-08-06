#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_SPLIT_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_SPLIT_HPP

#include <string_view>
#include <vector>

namespace runtime {
namespace string_utils {
namespace implementation {

template <typename Delimiter>
inline std::vector<std::string_view> split_implementation(std::string_view data, Delimiter&& delimiter)
{
    std::vector<std::string_view> splitted;
    splitted.reserve(data.length() / 4);
    size_t pos = 0;
    while (pos != std::string_view::npos) {
        pos = data.find_first_not_of(delimiter);
        if (pos == std::string_view::npos) {
            return splitted;
        }
        data = data.substr(pos);
        pos = data.find_first_of(delimiter);
        splitted.emplace_back(data.substr(0, pos));
        data = data.substr(pos == std::string_view::npos ? 0 : pos);
    }
    splitted.shrink_to_fit();
    return splitted;
}

//inline std::vector<std::string_view> split(std::string_view data, char delimiter)
//{
//    return split_implementation(data, delimiter);
//}
//
//inline std::vector<std::string_view> whitespace_split(std::string_view data)
//{
//    return split_implementation(data, " \f\n\r\t\v");
//}

}// namespace implementation
}// namespace string_utils
}// namespace runtime

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_SPLIT_HPP
