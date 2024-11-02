# Create output library
add_library(output STATIC output.cpp output.hpp)

# Include directories for input library
target_include_directories(output PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# For config.h
target_include_directories(output PUBLIC "${PROJECT_BINARY_DIR}")
