cmake_minimum_required(VERSION 3.3)
project(mergeable_heaps)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lgtest -O1 -O2 -O3 -O4 -std=c++11")

set(SOURCE_FILES main.cpp skew_heap.cpp leftist_heap.cpp binomial_heap.cpp)
add_executable(mergeable_heaps ${SOURCE_FILES})
