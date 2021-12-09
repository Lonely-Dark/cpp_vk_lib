#include "cpp_vk_lib/vk/events/wall_post_new.hpp"

#include "simdjson.h"
#include "time_measurement.hpp"

#include <fstream>

constexpr char wall_post_payload[] = R"__(
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

int main()
{
    std::ofstream benchmark_file("wall_post_new.txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    for (size_t total_events = 0; total_events < 50'000; total_events += 100) {
        std::string time_spent = time_measurement(total_events, []{
            simdjson::dom::parser parser;
            simdjson::dom::element event_object = parser.parse(wall_post_payload, strlen(wall_post_payload));
            vk::event::wall_post_new event(event_object);
        });
        benchmark_file << time_spent << " " << total_events << std::endl << std::flush;
    }
}