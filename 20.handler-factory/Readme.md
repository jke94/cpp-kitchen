# 20.handler-factory
Abastact high level example to parsing XMLs.

## A. C++ example compilation with CMake.

1. Generate compilation files with CMake.
```
cmake -B ./build
```
2. Compilation process with CMake.
```
cmake --build ./build/
```
3. Run example.
```
./build/XMLHandlerExample
```

Program output:

```
~$ ./build/XMLHandlerExample
Handling XML with version 1.0
Handling XML with version 2.0
Error: Version not found
~$
```