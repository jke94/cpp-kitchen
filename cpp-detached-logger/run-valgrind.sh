#!/bin/bash

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=cpp-detached-logger.valgrind.txt \
         ./cpp-detached-logger