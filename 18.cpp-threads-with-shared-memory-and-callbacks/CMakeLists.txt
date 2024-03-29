cmake_minimum_required(VERSION 3.20)

project(monitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_SOURCE_DIR}/api)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/interfaces)

file(GLOB_RECURSE CLIENT_SRC_FILES RELATIVE 
    ${CMAKE_SOURCE_DIR} "src/*.cpp"
)

add_executable(main ${CLIENT_SRC_FILES})