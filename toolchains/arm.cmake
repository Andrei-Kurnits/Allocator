set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER "${CMAKE_SOURCE_DIR}/toolchains/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "${CMAKE_SOURCE_DIR}/toolchains/gcc-arm-none-eabi-10.3-2021.10/bin/arm-none-eabi-g++")

set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)