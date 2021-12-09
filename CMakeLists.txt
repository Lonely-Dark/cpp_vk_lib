cmake_minimum_required(VERSION 3.9)

project(
    cpp_vk_lib
    DESCRIPTION "C++ vk library"
    HOMEPAGE_URL "https://github.com/threadpoolexecutor/cpp_vk_lib"
    LANGUAGES CXX
)

string(
    COMPARE EQUAL
    "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
    IS_TOP_PROJECT
)

set(CMAKE_C_COMPILER /usr/bin/gcc CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER /usr/bin/g++ CACHE PATH "" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Link simdjson as shared library")
set(CPP_VK_LIB_STATIC_BUILD OFF CACHE BOOL "Link cpp_vk_lib as shared library by default")
set(CPP_VK_LIB_SANITIZE OFF CACHE BOOL "Enable address and CFI sanitizer for Clang")

add_subdirectory(modules)
add_subdirectory(cpp_vk_lib)

if (IS_TOP_PROJECT)
    add_subdirectory(tests)
    add_subdirectory(examples)
endif()
