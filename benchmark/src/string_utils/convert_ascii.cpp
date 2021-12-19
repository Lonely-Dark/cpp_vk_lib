#include "cpp_vk_lib/runtime/string_utils/implementation/convert_ascii.hpp"

#include "../benchmarking.hpp"

#include <fstream>

int main()
{
    std::ofstream benchmark_file(get_current_file_without_extension() + ".txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    const std::string payload(1'000, 'a');
    for (size_t iterations = 0; iterations < total_benchmark_iterations; ++iterations) {
        for (size_t total = 1; total < 10'000; total += 1'000) {
            std::string time_spent = time_measurement(total, [&] {
                std::string data = runtime::string_utils::ascii_to_lower(payload);
            });
            benchmark_file << time_spent << " " << string_length_to_mib(payload.length() * total) << std::endl << std::flush;
        }
    }
}
