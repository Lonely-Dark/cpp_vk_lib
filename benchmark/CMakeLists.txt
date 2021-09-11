find_library(cpp_vk_lib PATHS "../cpp_vk_lib")

include_directories(../cpp_vk_lib/include)
include_directories(../modules/spdlog/include)
include_directories(../modules/asio/asio/include)

set(EXECUTABLE_OUTPUT_PATH ../build)

add_compile_options(-fPIC -O2)

if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    add_compile_options(-Wno-deprecated -Wno-deprecated-declarations)
endif()

add_executable(curl_benchmark curl.cpp)
target_link_libraries(curl_benchmark cpp_vk_lib pthread)
