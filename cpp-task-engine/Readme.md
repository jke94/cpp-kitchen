# cpp-task-engine

A task engine to run task. Designed a API + clien that consumes the task engine.

The file `main.cpp` implements a Task Engine API based on a thread pool, providing asynchronous task submission through futures, exception handling, graceful shutdown, and execution metrics. A client consumes this API to simulate concurrent HTTP GET and POST requests, handling their responses and failures while collecting performance statistics.

## How to build

From `cpp-task-engine` folder run:

```sh
. ./build
```

## How to run

After build...

```
./main
```

## About useful scripts to validate memory leaks and performance issues with `Valgrind` framework.

### Using `valgrind` for memory leaks

```sh
. ./run-valgrind.sh
```
### Using `valgrind` with `heldgrind` to detect synchronization errors in C and C++ programs that use threads

```sh
. ./run-valgrind-heldgrind.sh
```

### Using `valgrind` with `heldgrind` to detect errors in multi-threaded shared-memory programs

```sh
. ./run-valgrind-drd.sh
```