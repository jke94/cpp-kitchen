cmake_minimum_required(VERSION 3.20)

project(XMLHandlerExample)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRC_FILES RELATIVE 
    ${CMAKE_SOURCE_DIR} "src/*.cpp"
)

add_executable(XMLHandlerExample
    ${SRC_FILES}
)