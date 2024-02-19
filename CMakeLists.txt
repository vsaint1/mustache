cmake_minimum_required(VERSION 3.15)

project(mustache VERSION 0.1.0 LANGUAGES CXX C DESCRIPTION "OS library for memory access, Window management and overlay drawing." HOMEPAGE_URL https://github.com/vsaint1/mustache)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/build/generators/conan_toolchain.cmake)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

find_package(SPDLOG REQUIRED)
find_package(ZLIB REQUIRED)


if(JAVA_LIB)

    find_package(JNI REQUIRED)
    include_directories(${JNI_INCLUDE_DIRS})

endif()


if(PYTHON_LIB)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/pybind11)
        message(FATAL_ERROR "Submodule pybind11 not found. Please run `git submodule update --init`")
    endif()

    add_subdirectory(external/pybind11)

    pybind11_add_module(${PROJECT_NAME} main.cpp ${SOURCES})

endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB spdlog::spdlog ntdll.lib)
