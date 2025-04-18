cmake_minimum_required(VERSION 3.10)
project(gRPCExample)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Generate server executable

add_executable(server 
    src/server.cpp 
    src_auto_proto/greeter.pb.cc 
    src_auto_proto/greeter.grpc.pb.cc
)
target_include_directories(server PRIVATE 
    proto 
    src_auto_proto
)
target_link_libraries(server PRIVATE gRPC::grpc++ protobuf::libprotobuf)

# Generate client executable
add_executable(client 
    src/client.cpp 
    src_auto_proto/greeter.pb.cc 
    src_auto_proto/greeter.grpc.pb.cc
)
target_include_directories(client PRIVATE proto src_auto_proto)
target_link_libraries(client PRIVATE gRPC::grpc++ protobuf::libprotobuf)
