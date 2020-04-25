# HollowCore
A cross-platform and cross-language object system written in C11.

HollowCore is comprised of a core module of the same name as well as modules for compatibility with other programming languares, user interfacing, ray tracing and other modules. 

The core module defines a reference-counted memory model and object system for creating polymorphic opaque-types in pure C11 source code. It includes basic data types for booleans, integers, and real numbers defined in a language-agnostic way, with operations for approximate equality, hashing, and printing to data streams. The object system defines a base object (*HCObject*) and object subtypes for strings (*HCString*), numbers (*HCNumber*), and byte arrays (*HCData*). These objects wrap basic C types of the same usage and provide additional facilities for working with them. They also enable inclusion of these data in containers, with the core module providing an ordered-nonexclusive container (*HCList*), unordered-exclusive container (*HCSet*) and a key-value container (*HCMap*). These types can be serialized and deserialized in JSON format using (*HCJSON*). All these objects can be used in a multi-thread safe way by utilizing threads (*HCThread*), locking (*HCLock*), and conditions (*HCCondition*).

The core module also defines types for geometry and two dimensional graphics. It provides basic geometry for points (*HCPoint*), rectangles and sizes (*HCSize* and *HCRectangle*), curves and contours (*HCContour*), and paths (*HCPath*). These geometric primitives can be drawn to in-memory pixel rasters and serialized to streams using (*HCRaster*).

All of these objects are tested using unit and integration test cases using *ctest*. They are also verified for correct use of memory with a variety of tools including *valgrind* and *Instruments*. The status of continuous testing using *Travis* for the release version of the library may be monitored here:

[![Build Status](https://travis-ci.org/HollowCore/HollowCore.svg?branch=master)](https://travis-ci.org/HollowCore/HollowCore)

# macOS with Xcode

## Build and Test
* Install Xcode from App Store
* Clone repository from https://github.com/HollowCore/HollowCore
* Open HollowCore.xcodeproj
* Press Run button to build and debug HollowCore test cases

## Memory Testing
* Install Xcode from App Store
* Clone repository from https://github.com/HollowCore/HollowCore
* Open HollowCore.xcodeproj
* Click Product->Profile
* Select Leaks tool
* Press record button

## Code Coverage
TODO: Graphical coverage via Xcode. For now, use CLI method below.

## Documentation Generation
* Install Xcode from App Store
* Install doxygen from http://www.doxygen.nl/download.html
* Clone repository from https://github.com/HollowCore/HollowCore
* Open HollowCore.xcodeproj
* Select Doxygen build scheme from top-left menu
* Press run button
* Right-click HollowCore project in left menu and select Show in Finder
* Open html directory and double-click index.html

# macOS CLI with Brew

## Install Brew (also installs Xcode Command Line Tools)
```zsh
$ /bin/zsh -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

## Build and Test
```zsh
$ brew install cmake
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make -DCMAKE_BUILD_TYPE=Debug
$ make test
$ ctest --verbose
```

## Memory Testing (broken in macOS 10.15, use Instruments)
```zsh
$ brew install cmake valgrind
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ valgrind ./hollowcoretest
```

## Code Coverage
```zsh
$ brew install cmake gcovr lcov
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake .. -DCOVERAGE=1
$ make hollowcorecoverage
$ firefox html/index.html
```

## Documentation Generation
```zsh
$ brew install cmake doxygen graphviz
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ doxygen
$ open html/index.html
```

# Linux
* TODO: Linux graphical instructions via CLion. For now, use *Linux CLI with APT*.

# Linux CLI with APT

## Build and Test
```bash
$ sudo apt install build-essential cmake git
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make -DCMAKE_BUILD_TYPE=Debug
$ make test
$ ctest --verbose
```

## Memory Testing
```bash
$ sudo apt install build-essential cmake git valgrind
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
$ valgrind ./hollowcoretest
```

## Code Coverage

```bash
$ sudo apt install build-essential cmake git valgrind gcovr lcov genhtml
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake .. -DCOVERAGE=1
$ make hollowcorecoverage
$ firefox html/index.html
```

## Documentation Generation
```bash
$ sudo apt install build-essential cmake git doxygen graphviz
$ git clone https://github.com/HollowCore/HollowCore.git
$ cd HollowCore
$ mkdir build
$ cd build
$ cmake .. -DCOVERAGE=1
$ make hollowcorecoverage
$ firefox html/index.html
```

# Windows
* TODO: Windows graphical instructions via Visual Studio. For now, install Windows Subsystem for Linux and use *Linux CLI with APT*.

# Windows CLI
* TODO: Windows CLI instructions via gcc. For now, install Windows Subsystem for Linux and use *Linux CLI with APT*.

# License

MIT License

Copyright © 2020 HollowCore Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
