cmake_minimum_required(VERSION 3.16)
project(allocator
        VERSION 0.0.1
        DESCRIPTION "Pool memory allocator for embedded systems"
        HOMEPAGE_URL https://github.com/Andrei-Kurnits/Allocator
        LANGUAGES C)

include(CMakePrintSystemInformation)

# Check if necessary CMAKE variables are defined
if(NOT PAGE_SIZE)
    message(FATAL_ERROR "Size of page should be provided, e.g. 'cmake -DPAGE_SIZE=512'")
endif()
if(NOT POOL_SIZE)
    message(FATAL_ERROR "Size of heap should be provided, e.g. 'cmake -DPOOL_SIZE=512'")
endif()
if(NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Build type should be provided, e.g. 'cmake -DCMAKE_BUILD_TYPE=Release|Debug|Test'")
endif()
if(NOT BUILD_NUMBER)
    message(FATAL_ERROR "Build number should be provided, e.g. 'cmake -DBUILD_NUMBER=BABADEDA'")
endif()

# Add debug/test functions
if(${CMAKE_BUILD_TYPE} MATCHES "Debug|Test")
    set(ALLOCATOR_DEBUG 1)
    set(DEBUG_HEADER ${CMAKE_BINARY_DIR}/allocator_debug.h)
    configure_file(
        ${CMAKE_SOURCE_DIR}/src/allocator_debug.h.in
        ${DEBUG_HEADER}
    )
endif()

# Provide given configuration to sources
set(PUBLIC_HEADER ${CMAKE_BINARY_DIR}/allocator.h)
configure_file(
    ${CMAKE_SOURCE_DIR}/src/allocator.h.in
    ${PUBLIC_HEADER}
)

# Add port sources
if(${CMAKE_BUILD_TYPE} MATCHES "Test")
    message("Port necessary symbols will be defined in test")
else()
    if(NOT PORT)
        message(FATAL_ERROR "Port should be provided, e.g. 'cmake -DPORT=FreeRTOS'")
    endif()
    file(GLOB PORT_SOURCES
        "${CMAKE_SOURCE_DIR}/src/port/${PORT}/*.c"
    )
endif()

# Add target and its sources
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.c"
)
add_library(allocator STATIC ${SOURCES} ${PORT_SOURCES})
target_include_directories(allocator PRIVATE ${CMAKE_BINARY_DIR})

# Add port includes
target_include_directories(allocator PRIVATE ${CMAKE_SOURCE_DIR}/src/port)
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Test")
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/port/${PORT})
endif()

# Set C standard, compiler/linker flags
set_property(TARGET allocator PROPERTY C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wall -Wextra")
if(${CMAKE_BUILD_TYPE} MATCHES "Debug|Test")
    set(CMAKE_C_FLAGS "-g")
else()
    set(CMAKE_C_FLAGS_RELEASE "-Os")
endif()

# Copy the library and public header to output directory
if(NOT EXISTS ${PUBLIC_HEADER})
    message(FATAL_ERROR "Public header ${PUBLIC_HEADER} not found")
endif()
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/out/allocator-${CMAKE_BUILD_TYPE}-${CMAKE_PROJECT_VERSION}-${BUILD_NUMBER})
get_filename_component(PUBLIC_HEADER_FILE ${PUBLIC_HEADER} NAME)
add_custom_command(
        TARGET allocator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${PUBLIC_HEADER}
                ${OUTPUT_DIR}/${PUBLIC_HEADER_FILE})
add_custom_command(
        TARGET allocator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                $<TARGET_FILE:allocator>
                ${OUTPUT_DIR})

# Copy debug header to output directory
if(${CMAKE_BUILD_TYPE} MATCHES "Debug|Test")
    get_filename_component(DEBUG_HEADER_FILE ${DEBUG_HEADER} NAME)
    add_custom_command(
        TARGET allocator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${DEBUG_HEADER}
                ${OUTPUT_DIR}/${DEBUG_HEADER_FILE})
endif()

# Add unit tests
if(${CMAKE_BUILD_TYPE} MATCHES "Test")
    add_subdirectory(${CMAKE_SOURCE_DIR}/test)
endif()