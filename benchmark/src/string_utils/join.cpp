#include "cpp_vk_lib/runtime/string_utils/implementation/join.hpp"

#include "../benchmarking.hpp"

#include <fstream>
#include <vector>

int main()
{
    std::ofstream benchmark_file(get_current_file_without_extension() + ".txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    for (size_t total = 0; total < 1'000; total += 100) {
        const std::vector<std::string> elements(100, "frag mich warum");
        std::string time_spent = time_measurement(total, [&]{
            std::string data = runtime::string_utils::join(elements, ' ');
        });
        benchmark_file << time_spent << " " << total << std::endl << std::flush;
    }
}