cmake_minimum_required(VERSION 3.26)
project(lab04)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(lab04
        funcs.cpp
        header.h
        main.cpp)
