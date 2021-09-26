#include "cpp_vk_lib/vk/events/attachment_handler.hpp"

#include "simdjson.h"

static std::unique_ptr<vk::attachment::photo> get_photo(const simdjson::dom::element& attachment)
{
    return std::make_unique<vk::attachment::photo>(
        attachment["photo"]["owner_id"].get_int64(),
        attachment["photo"]["id"].get_int64());
}

static std::unique_ptr<vk::attachment::video> get_video(const simdjson::dom::element& attachment)
{
    return std::make_unique<vk::attachment::video>(
        attachment["video"]["owner_id"].get_int64(),
        attachment["video"]["id"].get_int64());
}

static std::unique_ptr<vk::attachment::document> get_doc(const simdjson::dom::element& attachment)
{
    return std::make_unique<vk::attachment::document>(
        attachment["doc"]["owner_id"].get_int64(),
        attachment["doc"]["id"].get_int64(),
        attachment["doc"]["url"].get_string());
}

static std::unique_ptr<vk::attachment::audio> get_audio(const simdjson::dom::element& attachment)
{
    return std::make_unique<vk::attachment::audio>(
        attachment["audio"]["owner_id"].get_int64(),
        attachment["audio"]["id"].get_int64());
}

static std::unique_ptr<vk::attachment::audio_message>
    get_audio_message(const simdjson::dom::element& attachment)
{
    return std::make_unique<vk::attachment::audio_message>(
        attachment["audio_message"]["owner_id"].get_int64(),
        attachment["audio_message"]["id"].get_int64(),
        attachment["audio_message"]["link_ogg"].get_string(),
        attachment["audio_message"]["link_mp3"].get_string());
}

static std::unique_ptr<vk::attachment::wall> get_wall(const simdjson::dom::element& attachment)
{
    return std::make_unique<vk::attachment::wall>(
        attachment["wall"]["from_id"].get_int64(),
        attachment["wall"]["id"].get_int64());
}

namespace vk {

std::vector<attachment::attachment_ptr_t>
    event::get_attachments(const simdjson::dom::array& attachments)
{
    std::vector<attachment::attachment_ptr_t> attachment_list;

    for (simdjson::dom::element attachment : attachments) {
        std::string type(attachment["type"]);
        if (type == "photo") {
            attachment_list.emplace_back(get_photo(attachment));
        } else if (type == "video") {
            attachment_list.emplace_back(get_video(attachment));
        } else if (type == "doc") {
            attachment_list.emplace_back(get_doc(attachment));
        } else if (type == "audio") {
            attachment_list.emplace_back(get_audio(attachment));
        } else if (type == "wall") {
            attachment_list.emplace_back(get_wall(attachment));
        } else if (type == "audio_message") {
            attachment_list.emplace_back(get_audio_message(attachment));
        }
    }

    return attachment_list;
}

}// namespace vk
