cmake_minimum_required(VERSION 3.12)
project(concat_iterator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(src)

add_executable(concat_iterator
        src/concat.hpp src/main.cpp)
