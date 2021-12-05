#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/attachment/attachment.hpp"

int main(int argc, char* argv[])
{
    SECTION(photo)
    {
        vk::attachment::photo photo(100, 200);

        TEST_CASE(photo.value() == "photo100_200");
        TEST_CASE(photo.type() == "photo");
    }
    SECTION(video)
    {
        vk::attachment::video video(100, 200);

        TEST_CASE(video.value() == "video100_200");
        TEST_CASE(video.type() == "video");
    }
    SECTION(document)
    {
        vk::attachment::document doc(100, 200, "https://www.something");

        TEST_CASE(doc.value() == "doc100_200");
        TEST_CASE(doc.type() == "doc");
        TEST_CASE(doc.raw_url() == "https://www.something");
    }
    SECTION(audio_message)
    {
        vk::attachment::audio_message audio_message(100, 200, "https://raw.ogg.link", "https://raw.mp3.link");

        TEST_CASE(audio_message.value() == "audio_message100_200");
        TEST_CASE(audio_message.type() == "audio_message");
        TEST_CASE(audio_message.raw_ogg() == "https://raw.ogg.link");
        TEST_CASE(audio_message.raw_mp3() == "https://raw.mp3.link");
    }
    SECTION(wall)
    {
        vk::attachment::wall wall(100, 200);

        TEST_CASE(wall.value() == "wall200_100");
        TEST_CASE(wall.type() == "wall");
    }
    return EXIT_SUCCESS;
}
