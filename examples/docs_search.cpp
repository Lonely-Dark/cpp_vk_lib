#include "vk/include/config/loader.hpp"
#include "vk/include/setup_logger.hpp"
#include "vk/include/attachment/attachment.hpp"
#include "vk/include/methods/utility/constructor.hpp"

#include <iostream>
#include <random>
#include <vector>

#include "simdjson.h"

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: ./docs_search <config.json> <query> <count>" << std::endl;
        return 1;
    }

    const char* query = argv[2];
    const char* count = argv[3];
    vk::config::load(argv[1]);
    vk::setup_logger("info");

    std::vector<vk::attachment::attachment_ptr_t> documents;
    const std::string raw_response = vk::method::group_constructor()
        .method("docs.search")
        .param("q",     query)
        .param("count", count)
        .perform_request();
    simdjson::dom::parser parser;
    const simdjson::dom::element response = parser.parse(raw_response);
    const simdjson::dom::array items = response["response"]["items"].get_array();
    documents.reserve(items.size());
    if (items.size() == 0) {
        spdlog::info("No documents found for your request");
        return 1;
    }
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<> distribution(0, items.size() - 1);
    for (size_t i = 0; i < items.size() && i < std::stoul(count); ++i) {
        const size_t index = distribution(generator);
        documents.push_back(std::make_shared<vk::attachment::document>(
            items.at(index)["owner_id"].get_int64(),
            items.at(index)["id"].get_int64(),
            items.at(index)["url"].get_string())
        );
    }
    for (auto const& attachment : documents) {
        spdlog::info("found: {}", attachment->value());
    }
    return 0;
}
