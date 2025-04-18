cmake_minimum_required(VERSION 3.20)
project(gRPCExample)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Generate server executable

add_executable(server 
    ${CMAKE_SOURCE_DIR} "src/server.cpp"
    ${CMAKE_SOURCE_DIR} "src_auto_proto/greeter.pb.cc"
    ${CMAKE_SOURCE_DIR} "src_auto_proto/greeter.grpc.pb.cc"
)
target_include_directories(server PRIVATE 
    proto 
    src_auto_proto
)
target_link_libraries(server PRIVATE 
    gRPC::grpc++ 
    protobuf::libprotobuf
)

# Generate client executable

add_executable(client 
    ${CMAKE_SOURCE_DIR} "src/client.cpp"
    ${CMAKE_SOURCE_DIR} "src_auto_proto/greeter.pb.cc"
    ${CMAKE_SOURCE_DIR} "src_auto_proto/greeter.grpc.pb.cc"
)
target_include_directories(client PRIVATE 
    proto 
    src_auto_proto
)
target_link_libraries(client PRIVATE 
    gRPC::grpc++ 
    protobuf::libprotobuf
)
