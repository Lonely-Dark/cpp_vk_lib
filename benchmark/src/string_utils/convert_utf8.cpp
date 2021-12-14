#include "cpp_vk_lib/runtime/string_utils/implementation/convert_utf8.hpp"

#include "../benchmarking.hpp"

#include <fstream>

int main()
{
    std::ofstream benchmark_file(get_current_file_without_extension() + ".txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    for (size_t total = 0; total < 2'000; total += 500) {
        const std::string payload(total, 'a');
        std::string time_spent = time_measurement(total, [&]{
            std::string data = runtime::string_utils::utf8_to_lower(payload);
        });
        benchmark_file << time_spent << " " << total << std::endl << std::flush;
    }
}
