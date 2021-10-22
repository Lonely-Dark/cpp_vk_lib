find_library(cpp_vk_lib PATHS "../cpp_vk_lib")

include_directories(../cpp_vk_lib/include)
include_directories(../modules/spdlog/include)
include_directories(../modules/asio/asio/include)

set(EXECUTABLE_OUTPUT_PATH ../build)

add_compile_options(-Wall -Wextra -Werror -Wunused -Wwrite-strings -Wimplicit-fallthrough -pedantic)
add_compile_options(-fPIC -O3 -flto)

if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    add_compile_options(-Wno-deprecated -Wno-deprecated-declarations)
endif()

add_executable(tests main.cpp)
target_link_libraries(tests simdjson gtest cpp_vk_lib)
