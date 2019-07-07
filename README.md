# HollowCore
A cross-platform and cross-language object system written in C11.

[![Build Status](https://travis-ci.org/HollowCore/HollowCore.svg?branch=master)](https://travis-ci.org/HollowCore/HollowCore)

# Build Requirements
```bash
$ sudo apt install build-essential cmake git
```

# Build
* Requires cmake and C compiler

```bash
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Test
* Requires cmake and C compiler

```bash
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ ctest --verbose
```

# Memory Test Requirements
```bash
$ sudo apt install build-essential cmake git valgrind
```

# Memory Testing
```bash
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ valgrind ./hollowcoretest
```

# Code Coverage
* Requires cmake, gcc or llvm, gcov, lcov, genhtml

```bash
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake .. -DCOVERAGE=1
$ make coverage
$ open coverage/html/index.html
```
