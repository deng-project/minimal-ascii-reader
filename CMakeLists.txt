# mar: Minimal ASCII reader for C++
# licence: Apache, see LICENCE file
# file: AsciiStreamReader.cpp - AsciiStreamReader class implementation
# author: Karl-Mihkel Ott

cmake_minimum_required(VERSION 3.10)
project(mar LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR_PLATFORM x64)

# Set appropriate compiler flags
if(MSVC)
    add_compile_options(/W3 /std:c++17)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

    # Set correct optimisation flags
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(/Od)
    else()
        add_compile_options(/O2)
    endif()
else()
    add_compile_options(-Wall -Wextra -std=c++17)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_compile_options(-Og)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Build libmar target
set(LIBMAR_TARGET mar)
set(LIBMAR_HEADERS
    include/AsciiLineReader.h
    include/AsciiStreamReader.h
    include/MARAssert.h)

set(LIBMAR_SOURCES 
    src/AsciiStreamReader.cpp
    src/AsciiLineReader.cpp)

add_library(${LIBMAR_TARGET} STATIC
    ${LIBMAR_HEADERS}
    ${LIBMAR_SOURCES})

target_include_directories(${LIBMAR_TARGET} PUBLIC include)