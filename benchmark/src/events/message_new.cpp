#include "cpp_vk_lib/vk/events/message_new.hpp"

#include "simdjson.h"
#include "../benchmarking.hpp"

#include <fstream>
#include <thread>

constexpr char message_new_payload[] = R"__(
    {
        "date":1614198737,
        "from_id":499047616,
        "id":0,
        "out":0,
        "peer_id":2000000008,
        "text":"123",
        "conversation_message_id":120470,
        "fwd_messages":[
        ],
        "important":false,
        "random_id":0,
        "attachments":[
           {
              "type":"photo",
              "photo":{
                 "album_id":-3,
                 "date":1614198736,
                 "id":457273210,
                 "owner_id":499047616,
                 "has_tags":false,
                 "access_key":"10cb094dbf37988149",
                 "sizes":[
                    {
                       "height":38,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=75x38&quality=96&sign=e2e4e9915fb7700bf90d37d3681fc67f&c_uniq_tag=aXD3iHEmwr3AH6mtjT4rDqBKSlF1p8VlPsZyKu2CVww&type=album",
                       "type":"s",
                       "width":75
                    },
                    {
                       "height":66,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=130x66&quality=96&sign=54ee71da89c00750777e0cdb19c7878c&c_uniq_tag=eBzjy_o1FM1acRcBZxa-9xLzlanagqcpdx6FncW1_A4&type=album",
                       "type":"m",
                       "width":130
                    },
                    {
                       "height":287,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=569x287&quality=96&proxy=1&sign=1c33322e6b6d9e4e2401be7e6e2dfd47&c_uniq_tag=S3TrLg2dMVuY9Du2iI0AoycCOIfOVfrp-J7vbnkGc0E&type=album",
                       "type":"x",
                       "width":569
                    },
                    {
                       "height":87,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=130x87&quality=96&crop=70,0,429,287&sign=5163c324e4b5a5efa8014e996d763a1a&c_uniq_tag=WJ_tyFj4B85jsJivvRejXd9PnQTDTBORhEiN7veKMpM&type=album",
                       "type":"o",
                       "width":130
                    },
                    {
                       "height":133,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=200x133&quality=96&crop=68,0,432,287&sign=d40872633146d9893386423fce8988bf&c_uniq_tag=lkQTCeaJTbukBQVoQRVM9RSQdEUUc3_9p3kXxtl9CYs&type=album",
                       "type":"p",
                       "width":200
                    },
                    {
                       "height":213,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=320x213&quality=96&crop=69,0,431,287&sign=22052d63c64f2cf36fbb774e53cc679a&c_uniq_tag=M5DsDfu1Ibam3BG2bkizSherSX3vbsiemLzKt4PVVGE&type=album",
                       "type":"q",
                       "width":320
                    },
                    {
                       "height":287,
                       "url":"https:\/\/sun9-72.userapi.com\/impg\/RDAqdqUJ4Xwl8czmg88U6wTHUl78-AGvfXfa5Q\/G_NgV3CMB0A.jpg?size=510x287&quality=96&crop=29,0,510,287&sign=1677d49d27458b79ce5e3d8dfc7b8706&c_uniq_tag=9NUb85z1HMhUT5uQD6qfbmWUIqbdCVxZMwHViYRe6jo&type=album",
                       "type":"r",
                       "width":510
                    }
                 ],
                 "text":""
              }
           }
        ],
        "is_hidden":false
    }
)__";

int main()
{
    std::ofstream benchmark_file(get_current_file_without_extension() + ".txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    for (size_t total_events = 0; total_events < 50'000; total_events += 2500) {
        std::string time_spent = time_measurement(total_events, []{
            simdjson::dom::parser parser;
            simdjson::dom::element event_object = parser.parse(message_new_payload, strlen(message_new_payload));
            vk::event::message_new event(event_object);
        });
        benchmark_file << time_spent << " " << total_events << std::endl << std::flush;
    }
}
