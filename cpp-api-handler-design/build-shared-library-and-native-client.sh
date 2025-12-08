#!/bin/bash

# Generate API shared library based on handler pattern.
g++  -fPIC \
    src/exampleApiLib/src/*.cpp \
    -std=c++17 \
    -Isrc/exampleApiLib/api \
    -Isrc/exampleApiLib/include \
    -shared -o exampleApiLib.so

# Generate main application that uses the API shared library.
g++ -fPIC \
    src/cpp-native-client/main.cpp \
    -std=c++17 \
    -Isrc/exampleApiLib/api \
    -L. \
    -Wl,-rpath=. \
    -l:exampleApiLib.so \
    -o main