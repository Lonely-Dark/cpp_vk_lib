#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_FORMAT_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_FORMAT_HPP

#include <array>
#include <sstream>

namespace runtime::string_utils {

template <typename... Args>
static std::string format(std::string_view data, Args&&... args)
{
    if (data.empty()) {
        return {};
    }

    static constexpr size_t average_word_size = 7;
    std::string formatted;
    formatted.reserve(data.size() + (average_word_size * sizeof...(args)));
    [[maybe_unused]] auto pack_one = [](auto&& argument) {
        if constexpr (std::is_integral_v<std::decay_t<decltype(argument)>>) {
            return std::to_string(argument);
        } else {
            return std::string(argument);
        }
    };
    std::array<std::string, sizeof...(Args)> elements{pack_one(args)...};
    for (size_t i = 0, curr = 0; i < data.size(); i++) {
        // If we're have '{}' token, insert parameter at this place.
        if (data[i] == '{' && data[i + 1] == '}') {
            formatted += elements[curr++];
            // Add all characters from source string except '{}' token.
        } else if (data[i - 1] != '{' || data[i] != '}') {
            formatted += data[i];
        }
    }
    return formatted;
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_FORMAT_HPP
