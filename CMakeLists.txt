cmake_minimum_required(VERSION 3.15)

project(mustache VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/pybind11)
    message(FATAL_ERROR "Submodule pybind11 not found. Please run `git submodule update --init`")
endif()

add_subdirectory(external/pybind11)

pybind11_add_module(mustache main.cpp)
