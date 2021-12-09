#include "cpp_vk_lib/vk/events/wall_reply_new.hpp"

#include "simdjson.h"
#include "time_measurement.hpp"

#include <fstream>

constexpr char wall_reply_new_payload[] = R"__(
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

int main()
{
    std::ofstream benchmark_file("wall_reply_new.txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    for (size_t total_events = 0; total_events < 50'000; total_events += 100) {
        std::string time_spent = time_measurement(total_events, []{
            simdjson::dom::parser parser;
            simdjson::dom::element event_object = parser.parse(wall_reply_new_payload, strlen(wall_reply_new_payload));
            vk::event::wall_reply_new event(event_object);
        });
        benchmark_file << time_spent << " " << total_events << std::endl << std::flush;
    }
}