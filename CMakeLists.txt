cmake_minimum_required(VERSION 3.31)
project(OpenPGN LANGUAGES C)

set(CMAKE_VERBOSE_MAKEFILE ON)

include(CTest)

option(ENABLE_COVERAGE "Enables coverage reporting" OFF)
if (ENABLE_COVERAGE)
  message(STATUS "Enables coverage reporting")
  add_compile_options(--coverage -g)
  add_link_options(--coverage -lgcov)
endif ()

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES lib/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(OpenPGN STATIC ${SOURCES} ${HEADERS})
target_include_directories(OpenPGN PUBLIC include)

add_subdirectory(test)
