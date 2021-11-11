function(initialize_submodule DIRECTORY)
  if(NOT EXISTS ${PROJECT_SOURCE_DIR}/${DIRECTORY}/.git)
    find_package(Git QUIET REQUIRED)
    message(STATUS "${PROJECT_SOURCE_DIR}/${DIRECTORY}/.git does not exist. Initializing ${DIRECTORY} submodule...")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init ${DIRECTORY}
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_EXIT_CODE)
    if(NOT GIT_EXIT_CODE EQUAL "0")
      message(FATAL_ERROR "${GIT_EXECUTABLE} submodule update --init modules/${DIRECTORY}
        failed with exit code ${GIT_EXIT_CODE}, please checkout submodules")
    endif()
  else()
    message(STATUS "${DIRECTORY} ready")
  endif()
endfunction(initialize_submodule)

initialize_submodule(modules/simdjson)
initialize_submodule(modules/spdlog)
initialize_submodule(modules/asio)

if (IS_TOP_PROJECT)
  message(STATUS "Run from top project so initialize gtest...")
  initialize_submodule(modules/googletest)
  set(BUILD_GMOCK OFF CACHE BOOL "Do not build gmock")
  add_subdirectory(googletest)
endif()

message(STATUS "Looking for CURL")
find_package(CURL REQUIRED)

if(CURL_FOUND)
  message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
  message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
  message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
else()
  message(STATUS "Fetching CURL")
  include(FetchContent)
  FetchContent_Declare(CURL GIT_REPOSITORY https://github.com/curl/curl.git)
  FetchContent_MakeAvailable(CURL)
endif()

add_subdirectory(simdjson EXCLUDE_FROM_ALL)
add_subdirectory(spdlog)
