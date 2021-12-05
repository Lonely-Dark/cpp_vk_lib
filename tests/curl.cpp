#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/runtime/net/network.hpp"

#include "simdjson.h"

#include <fstream>
#include <future>

int main(int argc, char* argv[])
{
    SECTION(POST_multithread)
    {
        std::vector<std::pair<std::thread, std::future<std::string>>> threads;
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            std::promise<std::string> promise;
            std::future<std::string> future = promise.get_future();
            std::thread thread([promise = std::move(promise)]() mutable {
                const auto received = runtime::network::request(runtime::network::require_data, "https://www.example.com");
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
        TEST_CASE(std::adjacent_find(responses.begin(), responses.end(), std::not_equal_to<>()) == responses.end());
    }
    SECTION(POST_require_data)
    {
        const auto received = runtime::network::request(runtime::network::require_data, "https://www.google.com");
        TEST_CASE(!received.error());
        const auto& result = received.value();
        TEST_CASE(!result.empty());
        TEST_CASE(result.find("Google") != std::string::npos);
    }
    SECTION(POST_omit_data)
    {
        const auto received = runtime::network::request(runtime::network::omit_data, "https://www.google.com");
        TEST_CASE(!received.error());
    }

    static auto raw_url_result = runtime::network::request(runtime::network::require_data, "https://api.thecatapi.com/v1/images/search");
    TEST_CASE(!raw_url_result.error());
    TEST_CASE(!raw_url_result.value().empty());
    simdjson::dom::parser parser;
    std::string cat_url = std::string(parser.parse(raw_url_result.value()).get_array().at(0)["url"]);

    SECTION(download_to_file)
    {
        TEST_CASE(runtime::network::download("buffer", cat_url) == 0);
        std::remove("buffer");
    }
    SECTION(download_to_buffer)
    {
        std::vector<uint8_t> bytes;
        TEST_CASE(runtime::network::download(bytes, cat_url) == 0);
    }
    SECTION(download_compare)
    {
        runtime::network::download("file", cat_url);
        std::ostringstream ss;
        ss << std::ifstream("file").rdbuf();
        const std::string& streambuf = ss.str();
        std::vector<uint8_t> file_buffer(streambuf.begin(), streambuf.end());

        std::vector<uint8_t> raw_buffer;
        runtime::network::download(raw_buffer, cat_url);

        TEST_CASE(file_buffer == raw_buffer);

        std::remove("file");
    }
    return EXIT_SUCCESS;
}
