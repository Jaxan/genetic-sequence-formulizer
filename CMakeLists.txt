project(Genetics)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c++1y)

find_package(Boost REQUIRED COMPONENTS filesystem program_options serialization system)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
set(libs ${libs} ${Boost_LIBRARIES})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory("lib")
add_subdirectory("src")
