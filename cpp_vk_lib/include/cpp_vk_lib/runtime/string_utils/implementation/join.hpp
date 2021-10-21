#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_JOIN_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_JOIN_HPP

#include <forward_list>
#include <list>
#include <numeric>
#include <string>

namespace runtime::string_utils::implementation {

template <typename T, typename Container, typename Delimiter>
void join_impl(std::string& output, Container&& container, Delimiter&& delimiter)
{
    for (const auto& element : container) {
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
}

template <typename Container, typename Delimiter>
std::string join(Container&& container, Delimiter&& delimiter)
{
    using T = std::decay_t<decltype(*std::begin(std::declval<Container>()))>;
    std::string output;
    if constexpr (
        !std::is_same_v<std::decay_t<Container>, std::forward_list<T>> &&
        !std::is_same_v<std::decay_t<Container>, std::list<T>>) {
        static const size_t average_word_length = 20;
        output.reserve(container.size() * average_word_length);
    }
    join_impl<T>(output, container, delimiter);
    return output;
}

template <typename T, size_t Size, typename Delimiter>
std::string join(T (&container)[Size], Delimiter&& delimiter)
{
    std::string output;
    static const size_t average_word_length = 20;
    output.reserve(Size * average_word_length);
    join_impl<T>(output, container, delimiter);
    return output;
}

}// namespace runtime::string_utils::implementation

namespace runtime::string_utils {

template <typename Container>
std::string join(Container&& container, char delimiter)
{
    return implementation::join(std::forward<Container>(container), delimiter);
}

template <typename Container>
std::string join(Container&& container, std::string_view delimiter)
{
    return implementation::join(std::forward<Container>(container), delimiter);
}

template <typename T>
std::string join(std::initializer_list<T> container, char delimiter)
{
    return implementation::join(container, delimiter);
}

template <typename T>
std::string join(std::initializer_list<T> container, std::string_view delimiter)
{
    return implementation::join(container, delimiter);
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_JOIN_HPP
