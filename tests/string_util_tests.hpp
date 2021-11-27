#include "cpp_vk_lib/runtime/string_utils/string_utils.hpp"

#include <deque>
#include <forward_list>
#include <gtest/gtest.h>
#include <list>

namespace util = runtime::string_utils;

TEST(string_utils, utf_to_lower)
{
    ASSERT_EQ(util::utf8_to_lower("ААА"), "ааа");
    ASSERT_EQ(util::utf8_to_lower("ЯЯЯ"), "яяя");
    ASSERT_EQ(util::utf8_to_lower("ЭЭЭ"), "эээ");
    ASSERT_EQ(util::utf8_to_lower("111"), "111");
    ASSERT_EQ(util::utf8_to_lower("{!}"), "{!}");
}

TEST(string_utils, utf_to_upper)
{
    ASSERT_EQ(util::utf8_to_upper("ааа"), "ААА");
    ASSERT_EQ(util::utf8_to_upper("яяя"), "ЯЯЯ");
    ASSERT_EQ(util::utf8_to_upper("ффф"), "ФФФ");
    ASSERT_EQ(util::utf8_to_upper("ыыы"), "ЫЫЫ");
    ASSERT_EQ(util::utf8_to_upper("111"), "111");
    ASSERT_EQ(util::utf8_to_upper("{!}"), "{!}");
}

TEST(string_utils, ascii_to_lower)
{
    ASSERT_EQ(util::ascii_to_lower("AAA"), "aaa");
    ASSERT_EQ(util::ascii_to_lower("ZZZ"), "zzz");
    ASSERT_EQ(util::ascii_to_lower("FFF"), "fff");
    ASSERT_EQ(util::ascii_to_lower("111"), "111");
    ASSERT_EQ(util::ascii_to_lower("{!}"), "{!}");
}

TEST(string_utils, ascii_to_upper)
{
    ASSERT_EQ(util::ascii_to_upper("aaa"), "AAA");
    ASSERT_EQ(util::ascii_to_upper("zzz"), "ZZZ");
    ASSERT_EQ(util::ascii_to_upper("fff"), "FFF");
    ASSERT_EQ(util::ascii_to_upper("sss"), "SSS");
    ASSERT_EQ(util::ascii_to_upper("111"), "111");
    ASSERT_EQ(util::ascii_to_upper("{!}"), "{!}");
}

TEST(string_utils, join_by_string)
{
    ASSERT_EQ(util::join({1, 2, 3}, "__"), "1__2__3");
    ASSERT_EQ(util::join({1ULL, 2ULL, 3ULL}, "???"), "1???2???3");
    ASSERT_EQ(util::join({1, 2, 3}, "  "), "1  2  3");
    ASSERT_EQ(util::join({"1", "2", "3"}, ""), "123");
}

TEST(string_utils, initializer_list_join)
{
    ASSERT_EQ(util::join({1, 2, 3}, ','), "1,2,3");
    ASSERT_EQ(util::join({1ULL, 2ULL, 3ULL}, ','), "1,2,3");
    ASSERT_EQ(util::join({1, 2, 3}, ','), "1,2,3");
    ASSERT_EQ(util::join({"1", "2", "3"}, ','), "1,2,3");
}

TEST(string_utils, c_style_array_join)
{
    std::string string_array[] = {"1", "2", "3"};
    int int_array[]            = {1, 2, 3};
    ASSERT_EQ(util::join(string_array, ','), "1,2,3");
    ASSERT_EQ(util::join(int_array, ','), "1,2,3");
}

TEST(string_utils, vector_join)
{
    const std::vector<std::string> string_container{"1", "2", "3"};
    const std::vector<int> int_container{1, 2, 3};
    ASSERT_EQ(util::join(string_container, ','), "1,2,3");
    ASSERT_EQ(util::join(int_container, ','), "1,2,3");
}

TEST(string_utils, deque_join)
{
    const std::deque<std::string> string_container{"1", "2", "3"};
    const std::deque<int> int_container{1, 2, 3};
    ASSERT_EQ(util::join(string_container, ','), "1,2,3");
    ASSERT_EQ(util::join(int_container, ','), "1,2,3");
}

TEST(string_utils, list_join)
{
    const std::list<std::string> string_container{"1", "2", "3"};
    const std::list<int> int_container{1, 2, 3};
    ASSERT_EQ(util::join(string_container, ','), "1,2,3");
    ASSERT_EQ(util::join(int_container, ','), "1,2,3");
}

TEST(string_utils, forward_list_join)
{
    const std::forward_list<std::string> string_container{"1", "2", "3"};
    const std::forward_list<int> int_container{1, 2, 3};
    ASSERT_EQ(util::join(string_container, ','), "1,2,3");
    ASSERT_EQ(util::join(int_container, ','), "1,2,3");
}

TEST(string_utils, array_join)
{
    const std::array<std::string, 3> string_container{"1", "2", "3"};
    const std::array<int, 3> int_container{1, 2, 3};
    ASSERT_EQ(util::join(string_container, ','), "1,2,3");
    ASSERT_EQ(util::join(int_container, ','), "1,2,3");
}

TEST(string_utils, split_by_symbol)
{
    const std::vector<std::string_view> v1{"1", "2", "3"};
    const std::vector<std::string_view> v2{"!", "?", ":"};
    const std::vector<std::string_view> v3{"1"};
    const std::vector<std::string_view> v4{"1", "1", "1", "1"};
    const std::vector<std::string_view> v5{"text1", "text2", "text3"};

    ASSERT_EQ(util::split("1 2 3", ' '), v1);
    ASSERT_EQ(util::split("! ? :", ' '), v2);
    ASSERT_EQ(util::split("1", ' '), v3);
    ASSERT_EQ(util::split("1?1?1?1", '?'), v4);
    ASSERT_EQ(util::split("text1,text2,text3", ','), v5);
}

TEST(string_utils, split_by_string)
{
    const std::vector<std::string_view> v1{"1", "2", "3"};
    const std::vector<std::string_view> v2{"!", "?", ":"};
    const std::vector<std::string_view> v3{"1"};
    const std::vector<std::string_view> v4{"1", "1", "1", "1"};
    const std::vector<std::string_view> v5{"text1", "text2", "text3"};

    ASSERT_EQ(util::split("1!?2!?3", "!?"), v1);
    ASSERT_EQ(util::split("!\\\\?\\\\:", "\\\\"), v2);
    ASSERT_EQ(util::split("1", " "), v3);
    ASSERT_EQ(util::split("1??1??1??1", "??"), v4);
    ASSERT_EQ(util::split("text1,,,text2,,,text3", ",,,"), v5);
}

TEST(string_utils, whitespace_split)
{
    const std::vector<std::string_view> v1{"1", "2", "3"};
    const std::vector<std::string_view> v2{"!", "?", ":"};
    const std::vector<std::string_view> v3{"1"};

    ASSERT_EQ(util::whitespace_split("1\n\n\n\t\r\v\f2   3"), v1);
    ASSERT_EQ(util::whitespace_split("!\n\n\n\t\r\v\f?\n\n\n\t\r\v\f:"), v2);
    ASSERT_EQ(util::whitespace_split("\n\n\n\t\r\v\f1\n\n\n\t\r\v\f"), v3);
}

TEST(string_utils, lazy_split)
{
    const std::array<const char*, 3> assertion_output_1 = {"1", "2", "3"};
    const std::array<const char*, 3> assertion_output_2 = {"\x60", "\x60", "\x60"};
    size_t offset_1                                     = 0;
    size_t offset_2                                     = 0;
    for (auto word : util::lazy_split("1___2___3", "___")) { ASSERT_EQ(word, assertion_output_1[offset_1++]); }
    for (auto word : util::lazy_split("\x60\x32\x60\x32\x60", "\x32")) {
        ASSERT_EQ(word, assertion_output_2[offset_2++]);
    }
}

TEST(string_utils, split_equality)
{
    const std::vector<std::string_view> assertion_output = {"\t\t\t\n", "\t\t\n\t", "\t\n\t\t"};
    constexpr std::string_view payload                   = "\t\t\t\n\v\t\t\n\t\v\t\n\t\t";
    std::vector<std::string_view> lazy_split_output;
    for (auto element : util::lazy_split(payload, "\v")) { lazy_split_output.push_back(element); }
    std::vector<std::string_view> regular_split_output = util::split(payload, "\v");
    ASSERT_EQ(lazy_split_output, regular_split_output);
    ASSERT_EQ(regular_split_output, assertion_output);
}

inline std::string create_split_buffer(size_t strings, size_t string_size, char payload)
{
    std::string output;
    output.reserve(strings * string_size);
    const std::string placeholder(string_size, payload);
    for (size_t i = 0; i < strings; ++i) {
        output += placeholder;
        output += " ";
    }
    output.pop_back();
    return output;
}

inline void dump_string_size(std::string_view input)
{
    std::cout << "             size: " << (static_cast<double>(input.size() * sizeof(char)) / 1024.0 / 1024.0)
              << " MiB." << std::endl;
}

TEST(string_utils, split_by_char_speed_test)
{
    const std::string buffer = create_split_buffer(5'000'000, 10, 'x');
    dump_string_size(buffer);
    util::split(buffer, ' ');
}

TEST(string_utils, split_by_string_speed_test)
{
    const std::string buffer = create_split_buffer(5'000'000, 10, 'x');
    dump_string_size(buffer);
    util::split(buffer, " ");
}

TEST(string_utils, lazy_split_speed_test)
{
    const std::string buffer = create_split_buffer(5'000'000, 10, 'x');
    dump_string_size(buffer);
    util::lazy_split(buffer, " ");
}

TEST(string_utils, format_speed_test)
{
    std::cout << "             format 60 MiB of data" << std::endl;
    for (size_t i = 0; i < 5'000'000; ++i) { fmt::format("params={}{}{}{}{}", "a", "b", "c", "d", "e"); }
}

TEST(string_utils, join_speed_test)
{
    std::cout << "             join 75 MiB of data\n";
    const std::vector<std::string> elements(5'000'000, "frag mich warum");
    util::join(elements, ' ');
}
