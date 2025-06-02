cmake_minimum_required(VERSION 3.23)

set(project "lab02")
project(${project})

set(CMAKE_CXX_STANDARD 17)

set(${project}_SOURCES
        main.cpp)

add_subdirectory(planet_lib)

add_subdirectory(goods)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES})

add_executable(${project}
        ${${project}_SOURCE_LIST})

target_link_libraries(${project} planet_lib)

target_link_libraries(${project} goods)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/sunsys.txt
        ${CMAKE_CURRENT_BINARY_DIR}  COPYONLY)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/goods.txt
        ${CMAKE_CURRENT_BINARY_DIR}  COPYONLY)
