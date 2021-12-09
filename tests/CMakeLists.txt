find_library(cpp_vk_lib PATHS "../cpp_vk_lib")

include_directories(../cpp_vk_lib/include)

add_compile_options(-fPIC -flto -O3)

enable_testing()

function(add_cpp_vk_lib_test bin_name path)
    if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        add_compile_options(-Wno-deprecated -Wno-deprecated-declarations)
    endif()
    add_executable(${bin_name} ${path})
    target_link_libraries(${bin_name} PRIVATE simdjson)
    target_link_libraries(${bin_name} PUBLIC cpp_vk_lib)
    add_test(
        NAME ${bin_name}
        COMMAND ${bin_name}
    )
endfunction()

file(GLOB test_files LIST_DIRECTORIES FALSE "*.cpp")
foreach(file ${test_files})
    get_filename_component(name ${file} NAME_WE)
    add_cpp_vk_lib_test(${name} ${file})
endforeach()
