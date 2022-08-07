#include "cpp_vk_lib/runtime/setup_logger.hpp"
#include "cpp_vk_lib/runtime/net/network.hpp"
#include "cpp_vk_lib/vk/long_poll/long_poll.hpp"
#include "cpp_vk_lib/vk/config/config.hpp"
#include "cpp_vk_lib/vk/events/message_new.hpp"
#include "cpp_vk_lib/vk/methods/basic.hpp"

#include <iostream>

inline bool cpp_vk_lib_curl_verbose = true;

void download_all_audio_messages(const vk::event::common& event)
{
    namespace net = runtime::network;

    if (event.type() != vk::event::type::message_new)
        return;

    auto message = event.get_message_new();

    if (message.text() != "/download_voice")
        return;

    auto download = [](const vk::attachment::base *attachment) {
        auto *voice_message = dynamic_cast<const vk::attachment::audio_message *>(attachment);
        if (!voice_message)
            return;
        net::download(voice_message->raw_mp3(), voice_message->value() + ".mp3");
    };

    for (const auto &attachment : message.attachments())
        download(attachment.get());

    if (message.has_fwd_messages())
        for (const auto &fwd_message : message.fwd_messages())
            for (const auto &attachment : fwd_message->attachments())
                download(attachment.get());

    if (message.has_reply())
        for (const auto &attachment : message.reply()->attachments())
            download(attachment.get());
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: ./download_audio_messages <config.json>" << std::endl;
        return 1;
    }

    vk::config::load(argv[1]);
    runtime::setup_logger(spdlog::level::level_enum::trace);

    asio::io_context io_context;
    vk::long_poll api(io_context);

    api.on_event(vk::event::type::message_new, download_all_audio_messages);
    api.run();
    return 0;
}
