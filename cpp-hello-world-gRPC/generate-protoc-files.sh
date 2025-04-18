which protoc
which grpc_cpp_plugin

mkdir -p src_auto_proto

protoc -I=./proto \
  --cpp_out=./src_auto_proto \
  --grpc_out=./src_auto_proto \
  --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
  ./proto/greeter.proto
