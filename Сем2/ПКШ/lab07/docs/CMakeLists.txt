cmake_minimum_required(VERSION 3.23)

set(project "docs")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        docs.cpp)

set(${project}_HEADERS
        docs.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS})

add_library(${project}
        STATIC
        ${${project}_SOURCE_LIST})
