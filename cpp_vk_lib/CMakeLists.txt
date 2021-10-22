add_compile_options(-Wall -Wextra -Werror -Wunused -Wwrite-strings -Wimplicit-fallthrough -Wpedantic)
add_compile_options(-fPIC -flto -O3 -ffast-math)

if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    add_compile_options(-Wno-deprecated -Wno-deprecated-declarations)
    #[[I don't know why these directories are not included in FreeBSD where i've tested this library]]
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

set(CPP_VK_LIB_SOURCE
    ../cpp_vk_lib/src/vk/implementation.cpp
    ../cpp_vk_lib/src/runtime/implementation.cpp)

if (CPP_VK_LIB_STATIC_BUILD)
    add_library(cpp_vk_lib STATIC ${CPP_VK_LIB_SOURCE})
else()
    add_library(cpp_vk_lib SHARED ${CPP_VK_LIB_SOURCE})
endif()

target_link_libraries(cpp_vk_lib PRIVATE curl)
target_link_libraries(cpp_vk_lib PRIVATE simdjson)
#[[Must be linked as PUBLIC to be able change logging level from user code]]
target_link_libraries(cpp_vk_lib PUBLIC spdlog)
if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    target_link_libraries(cpp_vk_lib PUBLIC execinfo)
endif()
if (CMAKE_CXX_COMPILER MATCHES ".*mingw.*")
    target_link_libraries(cpp_vk_lib PUBLIC ws2_32)
endif()

target_include_directories(cpp_vk_lib PRIVATE include)
target_include_directories(cpp_vk_lib PRIVATE ../modules/spdlog/include)
target_include_directories(cpp_vk_lib PRIVATE ../modules/asio/asio/include)
