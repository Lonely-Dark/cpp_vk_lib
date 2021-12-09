#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/runtime/net/network.hpp"

#include "simdjson.h"

#include <fstream>
#include <future>
#include <iostream>

int main(int argc, char* argv[])
{
    setup_cpp_vk_lib_environment();

    SECTION(POST_require_data)
    {
        runtime::network::request_context ctx;
        ctx.output_needed = true;
        ctx.host = "https://www.google.com";
        ctx.target = std::make_optional<std::map<std::string, std::string>>();
        const auto received = runtime::network::request(ctx);
        TEST_CASE(!received.error());
        const auto& result = received.value();
        TEST_CASE(!result.empty());
        TEST_CASE(result.find("Google") != std::string::npos);
    }
    SECTION(POST_omit_data)
    {
        runtime::network::request_context ctx;
        ctx.output_needed = true;
        ctx.host = "https://www.google.com";
        ctx.target = std::make_optional<std::map<std::string, std::string>>();
        const auto received = runtime::network::request(ctx);
        TEST_CASE(!received.error());
    }
    runtime::network::request_context ctx;
    ctx.output_needed = true;
    ctx.host = "https://api.thecatapi.com/v1/images/search";
    ctx.target = std::make_optional<std::map<std::string, std::string>>();
    const auto raw_url_result = runtime::network::request(ctx);
    TEST_CASE(!raw_url_result.error());
    TEST_CASE(!raw_url_result.value().empty());
    simdjson::dom::parser parser;
    std::string cat_url = std::string(parser.parse(raw_url_result.value()).get_array().at(0)["url"]);

    SECTION(download_to_file)
    {
        runtime::network::request_context cat_ctx;
        cat_ctx.io_filename = "buffer";
        cat_ctx.io_server = cat_url;
        TEST_CASE(runtime::network::download(cat_ctx) == 0);
        // Don't remove this file, so we need this file to compare below.
    }

    std::vector<uint8_t> bytes;

    SECTION(download_to_buffer)
    {
        runtime::network::request_context cat_ctx;
        cat_ctx.io_buffer_ptr = &bytes;
        cat_ctx.io_server = cat_url;
        TEST_CASE(runtime::network::download(cat_ctx) == 0);
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
