# Create mesh library
add_library(mesh STATIC mesh.cpp mesh.hpp)

# Include directories for mesh library
target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})