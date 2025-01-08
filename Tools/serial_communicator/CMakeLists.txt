cmake_minimum_required(VERSION 3.10)

project(serial)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR})

# file(GLOB_RECURSE SRCFILES ${CMAKE_SOURCE_DIR}/*.c)

file(GLOB SRCFILES ${CMAKE_SOURCE_DIR}/*.c)

add_executable(main ${SRCFILES})

# target_link_libraries(main m)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

