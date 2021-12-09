set(
    SRC
    src/vk/implementation.cpp
    src/runtime/implementation.cpp
)

if(CPP_VK_LIB_STATIC_BUILD)
    add_library(cpp_vk_lib STATIC ${SRC})
else()
    add_library(cpp_vk_lib SHARED ${SRC})
endif()

target_include_directories(cpp_vk_lib PRIVATE include)
target_include_directories(cpp_vk_lib PRIVATE ../modules/spdlog/include)
target_include_directories(cpp_vk_lib PRIVATE ../modules/asio/asio/include)

target_compile_options(
    cpp_vk_lib PRIVATE
    -Werror -Wall -Wextra -Wsign-compare -Wshadow -Wwrite-strings
    -Wpointer-arith -Winit-self -Wno-sign-conversion -Wredundant-decls
    -fPIC -flto -O3
)

if(CPP_VK_LIB_SANITIZE)
    message(STATUS "Compiling cpp_vk_lib with sanitizer flags")
    add_compile_options(
        -fsanitize=address -fno-omit-frame-pointer
        -fsanitize=undefined -fno-sanitize-recover=all
        -fsanitize-address-use-after-scope
    )
    link_libraries(
        -fsanitize=address -fno-omit-frame-pointer
        -fsanitize=undefined -fno-sanitize-recover=all
        -fsanitize-address-use-after-scope
    )
    if(CMAKE_CXX_COMPILER MATCHES "clang")
        add_compile_options(-fsanitize=cfa)
        link_libraries(-fsanitize=cfa)
    endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    target_compile_options(cpp_vk_lib PRIVATE -Wno-deprecated -Wno-deprecated-declarations)
    #[[I don't know why these directories are not included in FreeBSD where i've tested this library]]
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

target_link_libraries(cpp_vk_lib PRIVATE curl)
target_link_libraries(cpp_vk_lib PRIVATE simdjson)
target_link_libraries(cpp_vk_lib PUBLIC spdlog)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    target_link_libraries(cpp_vk_lib PUBLIC execinfo)
endif()

if(CMAKE_CXX_COMPILER MATCHES ".*mingw.*")
    target_link_libraries(cpp_vk_lib PUBLIC ws2_32)
endif()