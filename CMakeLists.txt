cmake_minimum_required(VERSION 3.12...3.18)

# Keep the source clean
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name and version
project(
  LILI
  VERSION 0.0
  DESCRIPTION "Multiscale plasma simulator"
  LANGUAGES CXX
)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    message(STATUS "No build type selected, default to ${CMAKE_BUILD_TYPE}")
endif()


# Set CMake helper location
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Version configuration
include(GetGitRevisionDescription)
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
git_local_changes(GIT_STATUS)
configure_file(cmake/config.h.in "${PROJECT_BINARY_DIR}/config.h")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.75.0)
message(STATUS "Boost version: ${Boost_VERSION}")

# Define sources
add_subdirectory(src)
# add_subdirectory(external)

# Options
option(UNIT_TEST "Build the unit tests" OFF)

# Enable Testing
# if(UNIT_TEST)
#   add_subdirectory(tests)
#   enable_testing()
# endif()
