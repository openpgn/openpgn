cmake_minimum_required(VERSION 3.31)
project(OpenPGN LANGUAGES C)

# COVERAGE REPORTING
include(CTest)
option(ENABLE_COVERAGE "Enables coverage reporting" OFF)
if (ENABLE_COVERAGE)
  message(STATUS "Enables coverage reporting")
  add_compile_options(--coverage -g)
  add_link_options(--coverage -lgcov)
endif ()

# PROJECT CONFIGURATION
set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES lib/*.c)
file(GLOB_RECURSE HEADERS include/*.h)

add_library(OpenPGN SHARED ${SOURCES} ${HEADERS})
target_compile_options(OpenPGN PUBLIC -fvisibility=hidden)
target_include_directories(OpenPGN PUBLIC include)

add_subdirectory(test)

# LINK-TIME-OPTIMIZATION
include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if( supported )
  message(STATUS "IPO / LTO enabled")
  set_property(TARGET OpenPGN PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "IPO / LTO not supported: <${error}>")
endif()
