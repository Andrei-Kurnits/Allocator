target_include_directories(allocator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(allocator PRIVATE ${CMAKE_SOURCE_DIR}/externals/FreeRTOS-Kernel/include)
target_include_directories(allocator PRIVATE ${CMAKE_SOURCE_DIR}/externals/FreeRTOS-Config)