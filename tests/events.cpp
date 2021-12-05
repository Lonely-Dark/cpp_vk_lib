#include "setup_testing_environment.hpp"

#include "cpp_vk_lib/vk/events/common.hpp"
#include "cpp_vk_lib/vk/events/message_new.hpp"
#include "cpp_vk_lib/vk/events/wall_post_new.hpp"
#include "cpp_vk_lib/vk/events/wall_reply_new.hpp"

#include "simdjson.h"

constexpr char message_new[] = R"__(
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

constexpr char wall_post_new[] = R"__(
    {
       "id":309,
       "from_id":-192764727,
       "owner_id":-192764727,
       "date":1614247897,
       "marked_as_ads":0,
       "post_type":"post",
       "text":"123",
       "can_edit":1,
       "created_by":499047616,
       "can_delete":1,
       "attachments":[
          {
             "type":"photo",
             "photo":{
                "album_id":-7,
                "date":1614247897,
                "id":457350091,
                "owner_id":-192764727,
                "has_tags":false,
                "access_key":"2dc49efc98fa525880",
                "post_id":309,
                "sizes":[
                   {
                      "height":70,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=75x70&quality=96&sign=62af754df69aec7a8c37f2546d7b6e14&c_uniq_tag=EDGfsupx-vLch4s_4Z3CNWib1oUy7_zcO02Rgr1dUf4&type=album",
                      "type":"s",
                      "width":75
                   },
                   {
                      "height":121,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=130x121&quality=96&sign=1a4b2dfb43a4df5d4787b6b8f1e85920&c_uniq_tag=uvOIIE-hXo8opdSWc8g5qkZy1SpTl4Vv1lZPEKa2H5k&type=album",
                      "type":"m",
                      "width":130
                   },
                   {
                      "height":315,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=339x315&quality=96&proxy=1&sign=4987ec76da4c0d2eb3dfd5e8dbd78b36&c_uniq_tag=Iut4Wp5M9bHTqZXpmcpMVAiYrhf6dzMiOjBN8rhOmSA&type=album",
                      "type":"x",
                      "width":339
                   },
                   {
                      "height":121,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=130x121&quality=96&sign=1a4b2dfb43a4df5d4787b6b8f1e85920&c_uniq_tag=uvOIIE-hXo8opdSWc8g5qkZy1SpTl4Vv1lZPEKa2H5k&type=album",
                      "type":"o",
                      "width":130
                   },
                   {
                      "height":186,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=200x186&quality=96&sign=b1e5ef6e1c343081e4353f22a0190886&c_uniq_tag=XnQgVSaFQtvrzY7F_m6xJRhJ1km7ksMoYrKou1G_tWU&type=album",
                      "type":"p",
                      "width":200
                   },
                   {
                      "height":297,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=320x297&quality=96&sign=9ee6586c94dfdebb47380edde52dfa5f&c_uniq_tag=bYdRy5CJR7F4cjP4JsOhBsdRrwvylqG86vYwRvBuWjM&type=album",
                      "type":"q",
                      "width":320
                   },
                   {
                      "height":315,
                      "url":"https:\/\/sun9-36.userapi.com\/impg\/Th_8SnovfRK0c_cFCQi2qmrirs9oAcgH7WaC5A\/ps47ZxhWhMs.jpg?size=339x315&quality=96&proxy=1&sign=4987ec76da4c0d2eb3dfd5e8dbd78b36&c_uniq_tag=Iut4Wp5M9bHTqZXpmcpMVAiYrhf6dzMiOjBN8rhOmSA&type=album",
                      "type":"r",
                      "width":339
                   }
                ],
                "text":"",
                "user_id":100
             }
          }
       ],
       "comments":{
          "count":0
       },
       "is_favorite":false
    }
)__";

constexpr char wall_reply_new[] = R"__(
    {
       "id":312,
       "from_id":499047616,
       "post_id":310,
       "owner_id":-192764727,
       "parents_stack":[
       ],
       "date":1614250994,
       "text":"123",
       "thread":{
          "count":0
       },
       "post_owner_id":-192764727
    }
)__";

int main(int argc, char* argv[])
{
    SECTION(basic_message_new)
    {
        simdjson::dom::parser parser;
        simdjson::dom::element event_object = parser.parse(message_new, strlen(message_new));
        vk::event::message_new event(event_object);
        TEST_CASE("123" == event.text());
        TEST_CASE(499047616 == event.from_id());
        TEST_CASE(2000000008 == event.peer_id());
        TEST_CASE(120470 == event.conversation_message_id());
        TEST_CASE(!event.has_action());
        TEST_CASE(!event.has_fwd_messages());
        TEST_CASE(!event.has_reply());
    }
    SECTION(message_new_with_attachments)
    {
        simdjson::dom::parser parser;
        simdjson::dom::element event_object = parser.parse(message_new, strlen(message_new));
        vk::event::message_new event(event_object);
        auto photo_attachment = std::move(event.attachments()[0]);
        TEST_CASE(event.attachments().size() == 1);
        TEST_CASE(photo_attachment->type() == "photo");
        TEST_CASE(photo_attachment->value() == "photo499047616_457273210");
    }
    SECTION(basic_wall_post_new)
    {
        simdjson::dom::parser parser;
        simdjson::dom::element event_object = parser.parse(wall_post_new, strlen(wall_post_new));
        vk::event::wall_post_new event(event_object);
        TEST_CASE(event.id() == 309);
        TEST_CASE(event.from_id() == -192764727);
        TEST_CASE(event.owner_id() == -192764727);
        TEST_CASE(!event.marked_as_ads());
        TEST_CASE(event.text() == "123");
        TEST_CASE(event.can_edit());
        TEST_CASE(event.can_delete());
        TEST_CASE(event.attachments().size() == 1);
        TEST_CASE(event.attachments()[0]->value() == "photo-192764727_457350091");
    }
    SECTION(basic_wall_reply_new)
    {
        simdjson::dom::parser parser;
        simdjson::dom::element event_object = parser.parse(wall_reply_new, strlen(wall_reply_new));
        vk::event::wall_reply_new event(event_object);
        TEST_CASE(event.id() == 312);
        TEST_CASE(event.from_id() == 499047616);
        TEST_CASE(event.post_id() == 310);
        TEST_CASE(event.owner_id() == -192764727);
        TEST_CASE(event.text() == "123");
    }
    return EXIT_SUCCESS;
}