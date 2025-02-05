cmake_minimum_required(VERSION 3.31)
project(OpenPGN C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES lib/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(OpenPGN STATIC ${SOURCES} ${HEADERS})
target_include_directories(OpenPGN PUBLIC include)

add_subdirectory(test)