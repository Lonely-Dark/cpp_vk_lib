#include "cpp_vk_lib/runtime/net/network.hpp"
#include "cpp_vk_lib/vk/methods/basic.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"
#include "simdjson.h"

#include <filesystem>
#include <fstream>
#include <future>
#include <gtest/gtest.h>

TEST(curl, POST_multithreaded)
{
    std::vector<std::pair<std::thread, std::future<std::string>>> threads;
    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        std::promise<std::string> promise;
        std::future<std::string> future = promise.get_future();
        std::thread thread([promise = std::move(promise)]() mutable {
            const auto received = runtime::network::request(
                runtime::network::require_data,
                "https://www.example.com");
            if (received.error()) {
                FAIL() << "error while HTTP GET";
            }
            promise.set_value(received.value());
        });
        threads.emplace_back(std::move(thread), std::move(future));
    }
    std::vector<std::string> responses;
    for (auto& el : threads) {
        auto thread = std::move(el.first);
        auto future = std::move(el.second);
        responses.emplace_back(future.get());
        thread.join();
    }
    ASSERT_EQ(
        std::adjacent_find(responses.begin(), responses.end(), std::not_equal_to<>()),
        responses.end());
}

TEST(curl, POST_require_data)
{
    const auto received =
        runtime::network::request(runtime::network::require_data, "https://www.google.com");
    if (received.error()) {
        FAIL() << "error while HTTP GET";
    }
    if (const auto result = received.value(); result.empty()) {
        FAIL() << "empty response got";
    } else {
        if (result.find("Google") == std::string::npos) {
            FAIL() << "HTTP output mismatch";
        }
    }
}

TEST(curl, POST_omit_data)
{
    const auto received =
        runtime::network::request(runtime::network::omit_data, "https://www.google.com");
    if (received.error()) {
        FAIL() << "error while HTTP GET";
    }
}

TEST(curl, GET_omit_data_speed_test)
{
    constexpr std::string_view url = "https://www.example.com";
    for (size_t i = 0; i < 5; ++i) {
        runtime::network::request(runtime::network::omit_data, url);
    }
}

TEST(curl, GET_require_data_speed_test)
{
    constexpr std::string_view url = "https://www.example.com";
    for (size_t i = 0; i < 5; ++i) {
        runtime::network::request(runtime::network::require_data, url);
    }
}

static std::string get_cat_url()
{
    static auto received = runtime::network::request(
        runtime::network::require_data,
        "https://api.thecatapi.com/v1/images/search");
    if (received.error() || received.value().empty()) {
        std::cerr << "Failed to get cat URL\n";
        exit(-1);
    }
    simdjson::dom::parser parser;
    return std::string(parser.parse(received.value()).get_array().at(0)["url"]);
}

TEST(curl, download_to_file)
{
    if (runtime::network::download("buffer", get_cat_url()) != 0) {
        FAIL() << "Failed to download to file";
    }
    std::remove("buffer");
}

TEST(curl, download_to_buffer)
{
    if (std::vector<uint8_t> raw_buffer;
        runtime::network::download(raw_buffer, get_cat_url()) != 0) {
        FAIL() << "Failed to download to buffer";
    }
}

TEST(curl, download_compare)
{
    const std::string url = get_cat_url();
    runtime::network::download("file", url);
    std::ostringstream ss;
    ss << std::ifstream("file").rdbuf();
    const std::string& streambuf = ss.str();
    std::vector<uint8_t> file_buffer(streambuf.begin(), streambuf.end());

    std::vector<uint8_t> raw_buffer;
    runtime::network::download(raw_buffer, url);

    ASSERT_EQ(file_buffer, raw_buffer);

    std::remove("file");
}
