# Create mesh library
add_library(mesh STATIC mesh.hpp mesh.cpp)

# Include directories for mesh library
target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link external libraries
target_link_libraries(mesh PUBLIC hdf5::hdf5)
