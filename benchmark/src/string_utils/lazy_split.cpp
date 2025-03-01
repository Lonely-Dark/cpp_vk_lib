#include "cpp_vk_lib/runtime/string_utils/implementation/lazy_split.hpp"

#include "../benchmarking.hpp"

#include <fstream>

inline std::string create_split_buffer(size_t strings, size_t string_size, char payload)
{
    std::string output;
    output.reserve(strings * string_size);
    const std::string placeholder(string_size, payload);
    for (size_t i = 0; i < strings; ++i) {
        output += placeholder;
        output += " ";
    }
    output.pop_back();
    return output;
}

int main()
{
    std::ofstream benchmark_file(get_current_file_without_extension() + ".txt");
    benchmark_file.clear();
    benchmark_file << "Total Seconds\n";

    const auto split_buffer = create_split_buffer(100, 10, 'x');

    for (size_t iterations = 1; iterations < total_benchmark_iterations; ++iterations) {
        for (size_t total = 0; total < 1'000'000; total += 5'000) {
            std::string time_spent = time_measurement(total, [&] {
                static_cast<void>(runtime::string_utils::lazy_split(split_buffer, " "));
            });
            benchmark_file << time_spent << " " << string_length_to_mib(split_buffer.size()) << std::endl << std::flush;
        }
    }
}
