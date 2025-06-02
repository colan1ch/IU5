cmake_minimum_required(VERSION 3.26)
project(lab05_b)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab05_b main.cpp
        bubble_sort.cpp
        min_sort.cpp
        sorts.h)
