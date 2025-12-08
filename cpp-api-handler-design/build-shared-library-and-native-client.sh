#!/bin/bash

# Generate API shared library based on handler pattern.
g++  -fPIC \
    -shared \
    -std=c++17 \
    src/exampleApiLib/src/*.cpp \
    -Isrc/exampleApiLib/api \
    -Isrc/exampleApiLib/include \
    -o exampleApiLib.so

# Generate main application that uses the API shared library.
g++ -fPIC \
    src/cpp-native-client/main.cpp \
    -std=c++17 \
    -Isrc/exampleApiLib/api \
    -L. \
    -Wl,-rpath=. \
    -l:exampleApiLib.so \
    -o cpp-native-client