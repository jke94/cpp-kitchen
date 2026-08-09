# cpp-task-engine

A task engine to run task. Designed a API + clien that consumes the task engine.

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