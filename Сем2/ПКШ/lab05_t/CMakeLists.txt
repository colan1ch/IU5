cmake_minimum_required(VERSION 3.23)

set(project "lab05_t")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        main.cpp)

add_subdirectory(VecnSet)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES})

add_executable(${project}
        ${${project}_SOURCE_LIST})

target_link_libraries(${project} VecnSet)
