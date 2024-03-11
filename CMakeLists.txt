cmake_minimum_required(VERSION 3.15)
project(mustache VERSION 0.1.0 LANGUAGES CXX C DESCRIPTION "(GHF) - Linux and Windows library for memory access, Window management and overlay drawing." HOMEPAGE_URL https://github.com/vsaint1/mustache)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/build/generators/conan_toolchain.cmake)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

find_package(ZLIB REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(nlohmann_json REQUIRED)

set(BINDING "static")

if(BINDING STREQUAL "static")
    add_library(${PROJECT_NAME} STATIC ${SOURCES})

    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)

    target_link_libraries(${PROJECT_NAME} spdlog::spdlog ZLIB::ZLIB glfw OpenGl32 nlohmann_json::nlohmann_json)
endif()

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

if(BINDING STREQUAL "static")

    enable_testing()

    file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    list(REMOVE_ITEM SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    file(GLOB_RECURSE test_files ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
    foreach(test_file ${test_files})
        get_filename_component(test_name ${test_file} ${SOURCE} NAME_WE)

        add_executable(${test_name} ${test_file})
        add_test(NAME ${test_name} COMMAND ${test_name})

        set_property(TARGET ${test_name} PROPERTY FOLDER tests)

        target_include_directories(${test_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)

        target_link_libraries(${test_name} spdlog::spdlog ZLIB::ZLIB glfw OpenGl32 nlohmann_json::nlohmann_json mustache)


    endforeach()

endif()