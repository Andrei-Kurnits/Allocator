# Allocator - lightweight pool allocator library for bare-metal embedded devices

## How to build
### Requirements
On host machine the following should be installed:
- cmake (3.16 or newer)
- make
- GCC

### Examples:
Once: *cd allocator && mkdir build && cd build*

1. cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/arm.cmake -DPORT=FreeRTOS -DPAGE_SIZE=128 -DPOOL_SIZE=128 -DCMAKE_BUILD_TYPE=Debug -DBUILD_NUMBER=ABCD1234 .. && make
2. cmake -DPAGE_SIZE=128 -DPOOL_SIZE=128 -DCMAKE_BUILD_TYPE=Test -DBUILD_NUMBER=ABCD1234 .. && make


### How to set pool size and page size?
Using CMake definitions: *cmake -DPAGE_SIZE=128 -DPOOL_SIZE=128*
POOL_SIZE is a number of pages!

### Which build types are supported?
Release, Debug, Test
To choose one set CMake definition: *cmake -DCMAKE_BUILD_TYPE=Test*

### Is cross-compilation supported?
Yes, e.g. to build for ARM architecture specify CMake toolchain file: cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/arm.cmake
To use your own toolchain plese add the symbolic link and CMake toolchain file to *allocator/toolchains* directory.

### What platforms (ports) are supported?
- FreeRTOS
Additional platforms (ports) might be added into *allocator/src/port/PORT_NAME*
Next PORT_NAME should be provided to CMake as cmake -DPORT=FreeRTOS

## Tests
Unit tests are included. CMocka library is using to conduct the tests.
To build test binary provide: -DCMAKE_BUILD_TYPE=Test
Selected port is ignored then.
Test binary is located in *allocator/build/test/AllocatorUnitTests*

### How to build
cmake -DPAGE_SIZE=128 -DPOOL_SIZE=128 -DCMAKE_BUILD_TYPE=Test -DBUILD_NUMBER=ABCD1234 .. && make
./test/AllocatorUnitTests

### Expected result
[==========] Running 4 test(s).
[ RUN      ] testSimpleAllocation
[       OK ] testSimpleAllocation
[ RUN      ] testCountFreePagesLeft
[       OK ] testCountFreePagesLeft
[ RUN      ] testAllPagesAllocated
[       OK ] testAllPagesAllocated
[ RUN      ] testBadAddress
[       OK ] testBadAddress
[==========] 4 test(s) run.
[  PASSED  ] 4 test(s).

