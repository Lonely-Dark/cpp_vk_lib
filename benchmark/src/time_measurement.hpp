#include <chrono>
#include <iomanip>
#include <iostream>

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
