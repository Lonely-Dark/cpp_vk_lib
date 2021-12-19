#include "cpp_vk_lib/runtime/string_utils/implementation/join.hpp"

#include "../benchmarking.hpp"

#include <fstream>
#include <vector>

int main()
{
    std::ofstream benchmark_file(get_current_file_without_extension() + ".txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    const std::vector<std::string> payload(100, "frag mich warum");
    for (size_t iterations = 0; iterations < total_benchmark_iterations; ++iterations) {
        for (size_t total = 1; total < 100'000; total += 1'000) {
            std::string time_spent = time_measurement(total, [&] {
                std::string data = runtime::string_utils::join(payload, ' ');
            });
            benchmark_file << time_spent << " " << string_length_to_mib(payload.size() * total) << std::endl << std::flush;
        }
    }
}
