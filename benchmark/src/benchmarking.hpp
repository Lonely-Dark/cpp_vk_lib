#include <chrono>
#include <iomanip>
#include <iostream>

static constexpr size_t total_benchmark_iterations = 5;

#define get_current_file_without_extension()                                                                           \
    [] {                                                                                                               \
        std::string file = __FILE__;                                                                                   \
        file             = file.substr(file.find_last_of('/') + 1);                                                    \
        file             = file.substr(0, file.find_last_of('.'));                                                     \
        std::cout << "run benchmark " << file << std::endl;                                                            \
        return file;                                                                                                   \
    }()

template <typename Function>
std::string time_measurement(size_t iterations, Function function)
{
    auto start = std::chrono::high_resolution_clock::now();
    while (iterations--) {
        function();
    }
    auto time_spent = std::chrono::high_resolution_clock::now() - start;
    return std::to_string(std::chrono::duration_cast<std::chrono::duration<float>>(time_spent).count());
}

inline float string_length_to_mib(size_t string_length) noexcept
{
    return /* B */ string_length / /* KiB */ 1024.0f / /* MiB */ 1024.0f;
}