#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/runtime/net/network.hpp"

#include "simdjson.h"

#include <fstream>
#include <future>
#include <iostream>

bool cpp_vk_lib_curl_verbose = false;

int main(int argc, char* argv[])
{
    setup_cpp_vk_lib_environment();
    namespace net = runtime::network;

    SECTION(POST_require_data)
    {
        auto [data, error] = net::request("https://www.google.com", {}, net::data_flow::require);
        TEST_CASE(!error);
        TEST_CASE(!data.empty());
        TEST_CASE(data.find("Google") != std::string::npos);
    }
    SECTION(POST_omit_data)
    {
        auto [data, error] = net::request("https://www.google.com", {}, net::data_flow::omit);
        TEST_CASE(!error);
        TEST_CASE(data.empty());
    }
    SECTION(request_data)
    {
        auto [data, error] = net::request_data(
            "https://pelevin.gpt.dobro.ai/generate/", R"__({"prompt":"text","length":50})__", net::data_flow::require);
        TEST_CASE(!error);
        TEST_CASE(!data.empty());
    }
    SECTION(request_multithread)
    {
        for (size_t _ = 0; _ < 4; ++_) {
            std::cout << "Curl multithread test case...\n";
            std::vector<std::pair<std::thread, std::future<std::string>>> threads;
            for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
                std::promise<std::string> promise;
                std::future<std::string> future = promise.get_future();
                std::thread thread([promise = std::move(promise)]() mutable {
                    auto[data, error] = net::request("https://www.example.com", {}, net::data_flow::require);
                    promise.set_value(std::move(data));
                });
                threads.emplace_back(std::move(thread), std::move(future));
            }
            std::vector<std::string> responses;
            for (auto& el: threads) {
                auto thread = std::move(el.first);
                auto future = std::move(el.second);
                responses.emplace_back(future.get());
                thread.join();
            }
            TEST_CASE(std::adjacent_find(responses.begin(), responses.end(), std::not_equal_to<>()) == responses.end());
        }
    }

    auto [cat_url, cat_error] = net::request(
        "https://api.thecatapi.com/v1/images/search", {}, net::data_flow::require);
    TEST_CASE(!cat_error);
    TEST_CASE(!cat_url.empty());
    simdjson::dom::parser parser;
    std::string cat_image = std::string(parser.parse(cat_url).get_array().at(0)["url"]);
    std::cout << cat_image << std::endl;

    std::vector<uint8_t> bytes;

    SECTION(download_to_buffer)
    {
        bool downloaded = net::download(cat_image, bytes);
        TEST_CASE(downloaded);
        std::cout << "Downloaded " << bytes.size() << " bytes to buffer\n";
    }

    SECTION(download_to_file)
    {
        bool downloaded = net::download(cat_image, "buffer");
        TEST_CASE(downloaded);
        // Don't remove this file, so we need this file to compare below.
    }

    SECTION(download_compare)
    {
        std::ostringstream ss;
        ss << std::ifstream("buffer").rdbuf();
        const std::string& streambuf = ss.str();
        std::vector<uint8_t> file_buffer(streambuf.begin(), streambuf.end());
        TEST_CASE(file_buffer == bytes);
        std::remove("buffer");
    }
    return EXIT_SUCCESS;
}
