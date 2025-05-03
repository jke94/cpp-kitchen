#!/bin/bash

g++ ./src/main.cpp \
    -I./src/api \
    -I./src/include \
    -std=c++11 \
    -o ./cpp-detached-logger