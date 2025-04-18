# cpp-hello-world-gRPC

## Setup environment

```
. ./setup-development-env.sh
```

## Generate protoc files

The following command will generate a folder (`src_auto_proto`) with gRPC auto generated code.

```
. ./generate-protoc-files.sh
```

## Generate example with CMake

- Generate makefiles.

```
cmake -B ./build
```
- Compile.

```
cmake --build ./buid
```

# How to run

1. Open a terminal and running:

```
./build/server
```
2. Open another terminal and running:

```
./build/client
```
## Useful information

- [gRPC C++ quick start](https://grpc.io/docs/languages/cpp/quickstart/)