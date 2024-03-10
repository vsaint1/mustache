cmake_minimum_required(VERSION 3.15)
project(mustache VERSION 0.1.0 LANGUAGES CXX C DESCRIPTION "(GHF) - Linux and Windows library for memory access, Window management and overlay drawing." HOMEPAGE_URL https://github.com/vsaint1/mustache)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/build/generators/conan_toolchain.cmake)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

find_package(ZLIB REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(nlohmann_json REQUIRED)


set(BINDING "${BINDING}" CACHE STRING "Binding type: java, python, dll")

if(BINDING STREQUAL "java")


    add_compile_definitions(JAVA_BINDING=TRUE)

    find_package(JNI REQUIRED)

    include_directories(${JNI_INCLUDE_DIRS})

    add_library(${PROJECT_NAME} SHARED ${SOURCES})

    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)

    target_link_libraries(${PROJECT_NAME} spdlog::spdlog ZLIB::ZLIB glfw OpenGl32 nlohmann_json::nlohmann_json)


endif()


if(BINDING STREQUAL "python")

    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/pybind11)
        message(FATAL_ERROR "Submodule pybind11 not found. Please run `git submodule update --init`")
    endif()

    add_compile_definitions(PYTHON_BINDING=TRUE)

    add_subdirectory(external/pybind11)

    pybind11_add_module(${PROJECT_NAME} src/main.cpp ${SOURCES} )

    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "mustache" SUFFIX ".pyd")

    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)

    target_link_libraries(${PROJECT_NAME} spdlog::spdlog ZLIB::ZLIB glfw OpenGl32 nlohmann_json::nlohmann_json)

endif()

if(BINDING STREQUAL "dll")

    add_compile_definitions(MUSTACHE_SHARED=TRUE)

    add_library(${PROJECT_NAME} SHARED ${SOURCES})

    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)

    target_link_libraries(${PROJECT_NAME} spdlog::spdlog ZLIB::ZLIB glfw OpenGl32 nlohmann_json::nlohmann_json)
endif()