#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/runtime/string_utils/string_utils.hpp"

#include <deque>
#include <forward_list>
#include <list>
#include <array>

int main(int argc, char* argv[])
{
    namespace su = runtime::string_utils;
    SECTION(utf_to_lower)
    {
        TEST_CASE(su::utf8_to_lower("ААА") == "ааа");
        TEST_CASE(su::utf8_to_lower("ЯЯЯ") == "яяя");
        TEST_CASE(su::utf8_to_lower("ЭЭЭ") == "эээ");
        TEST_CASE(su::utf8_to_lower("111") == "111");
        TEST_CASE(su::utf8_to_lower("{!}") == "{!}");
    }
    SECTION(utf_to_upper)
    {
        TEST_CASE(su::utf8_to_upper("ааа") == "ААА");
        TEST_CASE(su::utf8_to_upper("яяя") == "ЯЯЯ");
        TEST_CASE(su::utf8_to_upper("ффф") == "ФФФ");
        TEST_CASE(su::utf8_to_upper("ыыы") == "ЫЫЫ");
        TEST_CASE(su::utf8_to_upper("111") == "111");
        TEST_CASE(su::utf8_to_upper("{!}") == "{!}");
    }
    SECTION(ascii_to_lower)
    {
        TEST_CASE(su::ascii_to_lower("AAA") == "aaa");
        TEST_CASE(su::ascii_to_lower("ZZZ") == "zzz");
        TEST_CASE(su::ascii_to_lower("FFF") == "fff");
        TEST_CASE(su::ascii_to_lower("111") == "111");
        TEST_CASE(su::ascii_to_lower("{!}") == "{!}");
    }
    SECTION(ascii_to_upper)
    {
        TEST_CASE(su::ascii_to_upper("aaa") == "AAA");
        TEST_CASE(su::ascii_to_upper("zzz") == "ZZZ");
        TEST_CASE(su::ascii_to_upper("fff") == "FFF");
        TEST_CASE(su::ascii_to_upper("sss") == "SSS");
        TEST_CASE(su::ascii_to_upper("111") == "111");
        TEST_CASE(su::ascii_to_upper("{!}") == "{!}");
    }
    SECTION(join_by_string)
    {
        TEST_CASE(su::join({1, 2, 3}, "__") == "1__2__3");
        TEST_CASE(su::join({1ULL, 2ULL, 3ULL}, "???") == "1???2???3");
        TEST_CASE(su::join({1, 2, 3}, "  ") == "1  2  3");
        TEST_CASE(su::join({"1", "2", "3"}, "") == "123");
    }
    SECTION(join_by_initializer_list)
    {
        TEST_CASE(su::join({1, 2, 3}, ',') == "1,2,3");
        TEST_CASE(su::join({1ULL, 2ULL, 3ULL}, ',') == "1,2,3");
        TEST_CASE(su::join({1, 2, 3}, ',') == "1,2,3");
        TEST_CASE(su::join({"1", "2", "3"}, ',') == "1,2,3");
    }
    SECTION(join_c_style_array)
    {
        std::string string_array[] = {"1", "2", "3"};
        int int_array[] = {1, 2, 3};
        TEST_CASE(su::join(string_array, ',') == "1,2,3");
        TEST_CASE(su::join(int_array, ',') == "1,2,3");
    }
    SECTION(join_by_vector)
    {
        const std::vector<std::string_view> string_container{"1", "2", "3"};
        const std::vector<int> int_container{1, 2, 3};
        TEST_CASE(su::join(string_container, ',') == "1,2,3");
        TEST_CASE(su::join(int_container, ',') == "1,2,3");
    }
    SECTION(join_by_deque)
    {
        const std::deque<std::string_view> string_container{"1", "2", "3"};
        const std::deque<int> int_container{1, 2, 3};
        TEST_CASE(su::join(string_container, ',') == "1,2,3");
        TEST_CASE(su::join(int_container, ',') == "1,2,3");
    }
    SECTION(join_by_list)
    {
        const std::list<std::string_view> string_container{"1", "2", "3"};
        const std::list<int> int_container{1, 2, 3};
        TEST_CASE(su::join(string_container, ',') == "1,2,3");
        TEST_CASE(su::join(int_container, ',') == "1,2,3");
    }
    SECTION(join_by_forward_list)
    {
        const std::forward_list<std::string_view> string_container{"1", "2", "3"};
        const std::forward_list<int> int_container{1, 2, 3};
        TEST_CASE(su::join(string_container, ',') == "1,2,3");
        TEST_CASE(su::join(int_container, ',') == "1,2,3");
    }
    SECTION(split_by_symbol)
    {
        const std::vector<std::string_view> v1{"1", "2", "3"};
        const std::vector<std::string_view> v2{"!", "?", ":"};
        const std::vector<std::string_view> v3{"1"};
        const std::vector<std::string_view> v4{"1", "1", "1", "1"};
        const std::vector<std::string_view> v5{"text1", "text2", "text3"};

        TEST_CASE(su::split("1 2 3", ' ') == v1);
        TEST_CASE(su::split("! ? :", ' ') == v2);
        TEST_CASE(su::split("1", ' ') == v3);
        TEST_CASE(su::split("1?1?1?1", '?') == v4);
        TEST_CASE(su::split("text1,text2,text3", ',') == v5);
    }
    SECTION(split_by_string)
    {
        const std::vector<std::string_view> v1{"1", "2", "3"};
        const std::vector<std::string_view> v2{"!", "?", ":"};
        const std::vector<std::string_view> v3{"1"};
        const std::vector<std::string_view> v4{"1", "1", "1", "1"};
        const std::vector<std::string_view> v5{"text1", "text2", "text3"};

        TEST_CASE(su::split("1!?2!?3", "!?") == v1);
        TEST_CASE(su::split("!\\\\?\\\\:", "\\\\") == v2);
        TEST_CASE(su::split("1", " ") == v3);
        TEST_CASE(su::split("1??1??1??1", "??") == v4);
        TEST_CASE(su::split("text1,,,text2,,,text3", ",,,") == v5);
    }
    SECTION(whitespace_split)
    {
        const std::vector<std::string_view> v1{"1", "2", "3"};
        const std::vector<std::string_view> v2{"!", "?", ":"};
        const std::vector<std::string_view> v3{"1"};

        TEST_CASE(su::whitespace_split("1\n\n\n\t\r\v\f2   3") == v1);
        TEST_CASE(su::whitespace_split("!\n\n\n\t\r\v\f?\n\n\n\t\r\v\f:") == v2);
        TEST_CASE(su::whitespace_split("\n\n\n\t\r\v\f1\n\n\n\t\r\v\f") == v3);
    }
    SECTION(lazy_split)
    {
        const std::array<const char*, 3> assertion_output_1 = {"1", "2", "3"};
        const std::array<const char*, 3> assertion_output_2 = {"\x60", "\x60", "\x60"};
        size_t offset_1 = 0;
        size_t offset_2 = 0;
        for (auto word : su::lazy_split("1___2___3", "___")) {
            TEST_CASE(word == assertion_output_1[offset_1++]);
        }
        for (auto word : su::lazy_split("\x60\x32\x60\x32\x60", "\x32")) {
            TEST_CASE(word == assertion_output_2[offset_2++]);
        }
    }
    SECTION(split_equality)
    {
        const std::vector<std::string_view> assertion_output = {"\t\t\t\n", "\t\t\n\t", "\t\n\t\t"};
        constexpr std::string_view payload  = "\t\t\t\n\v\t\t\n\t\v\t\n\t\t";
        std::vector<std::string_view> lazy_split_output;
        for (auto element : su::lazy_split(payload, "\v")) {
            lazy_split_output.push_back(element);
        }
        std::vector<std::string_view> regular_split_output = su::split(payload, "\v");
        TEST_CASE(lazy_split_output == regular_split_output);
        TEST_CASE(regular_split_output == assertion_output);
    }
}