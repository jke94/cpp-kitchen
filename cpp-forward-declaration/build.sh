#!/bin/bash

# Compile the source files
g++ ./src/*.cpp \
    ./main.cpp \
    -I./api \
    -I./include \
    -o ./example_cpp_forward_declaration