cmake_minimum_requirement(VERSION 3.12)

# Project name and version
project(M VERSION 0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Defines sources
add_subdirectory(src)
add_subdirectory(external)

# Enable Testing
enable_testing()

# Version configuration
configure_file(cmake/config.h.in src/config.h)

add_executable(main src/main.cpp)

# Options
option(M_EnableTesting   "Build the unit tests")
