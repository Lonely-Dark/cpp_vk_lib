#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"
#include "cpp_vk_lib/vk/events/wall_post_new.hpp"

#include "simdjson.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <fstream>

inline bool cpp_vk_lib_curl_verbose = false;

int main(int argc, char* argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: ./wall_get <config.json> <group_id> <count of posts> <out file>" << std::endl;
        return 1;
    }

    const char* config_path = argv[1];
    const char* group_id = argv[2];
    const char* count_of_posts = argv[3];
    const char* out_path = argv[4];
    vk::config::load(config_path);
    runtime::setup_logger(spdlog::level::level_enum::trace);

    const std::string response = vk::method::user_constructor()
        .method("wall.get")
        .param("owner_id", group_id)
        .param("count",    count_of_posts)
        .perform_request();
    simdjson::dom::parser parser;
    const simdjson::dom::element parsed_posts = parser.parse(response);
    const simdjson::dom::array posts = parsed_posts["response"]["items"].get_array();
    std::ofstream out(out_path);
    for (auto post : posts) {
        auto post_object = vk::event::wall_post_new(post);
        out << "[POST]:\n\t";
        out << post_object.text();
        out << "\n\n";
    }
    spdlog::info("Total posts parsed: {}", posts.size());
    return 0;
}
