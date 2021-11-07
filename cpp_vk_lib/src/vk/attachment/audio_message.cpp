#include "cpp_vk_lib/vk/attachment/attachment.hpp"
#include "spdlog/spdlog.h"

namespace vk::attachment {

audio_message::audio_message(int32_t owner_id, int32_t id, std::string_view raw_ogg, std::string_view raw_mp3)
    : base("audio_message", owner_id, id)
    , raw_ogg_(raw_ogg)
    , raw_mp3_(raw_mp3)
{
    spdlog::trace("create attachment: {}, raw_ogg={}, raw_mp3={}", value(), this->raw_ogg(), this->raw_mp3());
}

const std::string& audio_message::raw_ogg() const noexcept
{
    return raw_ogg_;
}

const std::string& audio_message::raw_mp3() const noexcept
{
    return raw_mp3_;
}

}// namespace vk::attachment
