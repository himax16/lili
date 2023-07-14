cmake_minimum_required(VERSION 3.15...3.18)

# Project name and version
project(LILI
  VERSION
    0.0
  DESCRIPTION
    "Multiscale plasma simulator"
  LANGUAGES
    C CXX
)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "No build type selected, default to ${CMAKE_BUILD_TYPE}")
endif()

# Set CMake helper location
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Load helper files
include(FetchContent)
include(LILITools)
include(GetGitRevisionDescription)

# Keep the source clean
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# C++ standard
set_lili_cxx()

# MPI
set_lili_mpi()

# HDF5
set_lili_hdf5()

# Version configuration
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_STATUS)
configure_file("${CMAKE_MODULE_PATH}/config.h.in"
               "${PROJECT_BINARY_DIR}/config.h")
               
# Define sources
add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(docs)
# add_subdirectory(external)

# Options
option(BUILD_TESTING "Build tests" ON)

# Testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
