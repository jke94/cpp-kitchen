#!/bin/bash

g++ ./src/src/config/di_config.cpp \
    ./src/src/*.cpp \
    ./src/main.cpp \
    -I./src/api \
    -I./src/include/config \
    -I./src/include \
    -o ./cpp-di-container-sample