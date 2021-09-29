#ifndef RUNTIME_STRING_UTILS_IMPLEMENTATION_LAZY_SPLIT_HPP
#define RUNTIME_STRING_UTILS_IMPLEMENTATION_LAZY_SPLIT_HPP

// Thanks some-coder366 for the idea.
// https://github.com/some-coder366/cpp-lazy-split/blob/master/split_iterators.cpp

#include <string_view>

namespace runtime::string_utils {

struct end_split_iterator
{};

template <typename StringType>
class split_iterator
{
public:
    split_iterator(StringType source, StringType delimiter)
        : src_(std::forward<StringType>(source))
        , delimiter_(std::forward<StringType>(delimiter))
        , first_(0)
        , last_(src_.find(delimiter))
    {}

    split_iterator& operator++() noexcept
    {
        first_ = last_ + delimiter_.size();
        last_ = src_.find(delimiter_, first_);
        return *this;
    }

    StringType operator*() const noexcept
    {
        if (last_ != StringType::npos) {
            return src_.substr(first_, last_ - first_);
        }

        finished_ = true;

        return src_.substr(first_, src_.size() - first_);
    }

    friend constexpr bool operator!=(const split_iterator& iterator, end_split_iterator) noexcept
    {
        return !iterator.finished_;
    }

    friend constexpr bool operator!=(end_split_iterator, const split_iterator& iterator) noexcept
    {
        return !iterator.finished_;
    }

private:
    StringType src_;
    StringType delimiter_;
    size_t first_;
    size_t last_;
    mutable bool finished_ = false;
};

template <typename StringType>
class split_range
{
public:
    split_range(StringType source, StringType delim) noexcept
        : src_(std::forward<StringType>(source))
        , delimiter_(std::forward<StringType>(delim))
    {}

    auto begin() const noexcept
    {
        return split_iterator<StringType>{src_, delimiter_};
    }

    auto end() const noexcept
    {
        return end_split_iterator{};
    }

private:
    StringType src_;
    StringType delimiter_;
};

inline auto lazy_split(std::string_view data, std::string_view delimiter)
{
    return split_range<std::string_view>(data, delimiter);
}

}// namespace runtime::string_utils

#endif// RUNTIME_STRING_UTILS_IMPLEMENTATION_LAZY_SPLIT_HPP
