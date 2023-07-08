# Create mesh library
add_library(input STATIC mesh.cpp mesh.hpp)

# Include directories for mesh library
target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})