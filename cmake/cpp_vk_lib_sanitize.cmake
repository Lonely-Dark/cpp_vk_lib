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
    add_compile_options(
        -fsanitize=cfi -fvisibility=default -flto
    )
    link_libraries(
        -fsanitize=cfi -fvisibility=default -flto
    )
endif()
