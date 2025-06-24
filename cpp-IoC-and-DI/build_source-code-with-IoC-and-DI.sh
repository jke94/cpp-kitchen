#!/bin/bash

# A. Build the source code with IoC and DI client example.

g++ ./src/source-code-with-IoC-and-DI/client/main.cpp \
    ./src/source-code-with-IoC-and-DI/src/*.cpp \
    -I./src/source-code-with-IoC-and-DI/interfaces \
    -I./src/source-code-with-IoC-and-DI/include \
    -std=c++14 \
    -o source-code-with-IoC-and-DI

# B. Build the unit tests for the source code with IoC and DI.

g++ \
  ./src/source-code-with-IoC-and-DI/src/*.cpp \
  ./src/source-code-with-IoC-and-DI/test/unittests/*.cpp \
  ./src/source-code-with-IoC-and-DI/test/UnitTestsMain.cpp \
  -I./src/source-code-with-IoC-and-DI/interfaces \
  -I./src/source-code-with-IoC-and-DI/include \
  -I./src/source-code-with-IoC-and-DI/test/include \
  -I./src/source-code-with-IoC-and-DI/test/mocks \
  -I./src/source-code-with-IoC-and-DI/test \
  -std=c++14 \
  -pthread \
  $(pkg-config --cflags --libs gtest gmock) \
  -o source-code-with-IoC-and-DI-unit-tests