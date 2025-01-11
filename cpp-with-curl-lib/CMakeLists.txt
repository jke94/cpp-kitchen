cmake_minimum_required(VERSION 3.20)

project(demologger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

file(GLOB_RECURSE CLIENT_SRC_FILES RELATIVE 
    ${CMAKE_SOURCE_DIR} "Client/*.cpp"
)

add_executable(main ${CLIENT_SRC_FILES})

target_link_libraries(main ${CURL_LIBRARIES})
