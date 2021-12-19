#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/runtime/signal_handlers.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/methods/constructor.hpp"
#include "cpp_vk_lib/vk/events/wall_post_new.hpp"

#include "simdjson.h"
#include "spdlog/spdlog.h"

#include <iostream>
#include <fstream>

bool cpp_vk_lib_curl_verbose = false;

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: ./upload_audio_message <config.json> <file.mp3> <peer_id>" << std::endl;
        return 1;
    }

    const char* config_path = argv[1];
    const char* file_path = argv[2];
    const char* peer_id = argv[3];
    vk::config::load(config_path);
    runtime::setup_signal_handlers();
    runtime::setup_logger(spdlog::level::level_enum::trace);

    const std::string server = vk::method::user_constructor()
        .method("docs.getMessagesUploadServer")
        .param("peer_id", peer_id)
        .param("type", "audio_message")
        .perform_request();
    simdjson::dom::parser parser;
    const std::string upload_server(parser.parse(server)["response"]["upload_url"]);
    namespace net = runtime::network;

    auto [uploaded, upload_error] = runtime::network::upload(
        upload_server,
        "file",
        "application/octet-stream",
        file_path,
        net::data_flow::require
    );
    if (upload_error) {
        spdlog::error("Upload error");
        return 1;
    }
    const simdjson::dom::element parsed_upload = parser.parse(uploaded);
    const std::string file(parsed_upload["file"]);
    if (file.empty()) {
        return 1;
    }
    const std::string docs_save_response = vk::method::user_constructor()
        .method("docs.save")
        .param("file", file)
        .param("title", "voice")
        .perform_request();
    const simdjson::dom::element uploaded_document = parser.parse(docs_save_response);
    const simdjson::dom::element audio_message = uploaded_document["response"]["audio_message"];
    std::unique_ptr<vk::attachment::audio_message> doc =
        std::make_unique<vk::attachment::audio_message>(
            audio_message["owner_id"].get_int64(),
            audio_message["id"].get_int64(),
            audio_message["link_ogg"].get_string(),
            audio_message["link_mp3"].get_string());
    vk::method::user_constructor()
        .method("messages.send")
        .param("peer_id", std::to_string(2000000000L + std::stol(peer_id)))
        .param("random_id", "0")
        .param("attachment", doc->value())
        .request_without_output();
    return 0;
}
