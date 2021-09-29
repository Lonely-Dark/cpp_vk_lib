#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_JOIN_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_JOIN_HPP

#include <forward_list>
#include <list>
#include <numeric>
#include <string>

namespace runtime::string_utils {

template <typename T, typename Container, typename Delimiter>
static std::string create_joined_string(Container&& elements, Delimiter&& delimiter)
{
    std::string output;
    if constexpr (
        !std::is_same_v<std::decay_t<Container>, std::forward_list<std::decay_t<T>>> &&
        !std::is_same_v<std::decay_t<Container>, std::list<std::decay_t<T>>>) {
        static const size_t average_word_length = 20;
        output.reserve(elements.size() * average_word_length);
    }
    for (const auto& element : elements) {
        if constexpr (std::is_integral_v<T>) {
            output += std::to_string(element);
        } else {
            output += element;
        }
        output += delimiter;
    }
    if constexpr (std::is_integral_v<std::decay_t<Delimiter>>) {
        output.pop_back();
    } else {
        output.resize(output.size() - delimiter.size());
    }
    output.shrink_to_fit();
    return output;
}

template <typename T, typename Container>
std::string join(Container&& elements, char delimiter)
{
    return create_joined_string<T>(std::forward<Container>(elements), delimiter);
}

template <typename T, typename Container>
std::string join(Container&& elements, std::string_view delimiter)
{
    return create_joined_string<T>(std::forward<Container>(elements), delimiter);
}

template <typename T>
std::string join(std::initializer_list<T> elements, char delimiter)
{
    return create_joined_string<T>(elements, delimiter);
}

template <typename T>
std::string join(std::initializer_list<T> elements, std::string_view delimiter)
{
    return create_joined_string<T>(elements, delimiter);
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_JOIN_HPP
