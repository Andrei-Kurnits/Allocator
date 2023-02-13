include(CTest)

# Get CMOCKA
set(OLD_BUILD_STATIC_LIB ${BUILD_STATIC_LIB})
set(OLD_CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(BUILD_STATIC_LIB 1)
set(CMAKE_BUILD_TYPE Release)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cmocka-1.1.5)
set(CMAKE_BUILD_TYPE ${OLD_CMAKE_BUILD_TYPE})
set(BUILD_STATIC_LIB ${OLD_BUILD_STATIC_LIB})

# Test binary
add_executable(AllocatorUnitTests test.c)
set_property(TARGET AllocatorUnitTests PROPERTY C_STANDARD 11)
target_link_libraries(AllocatorUnitTests allocator)
target_include_directories(AllocatorUnitTests PRIVATE ${OUTPUT_DIR})
target_include_directories(AllocatorUnitTests PRIVATE ${CMAKE_SOURCE_DIR}/src/port)

# Link agains CMOCKA
target_link_libraries(AllocatorUnitTests ${CMOCKA_STATIC_LIBRARY})

add_test(NAME AllocatorUnitTests COMMAND AllocatorUnitTests)