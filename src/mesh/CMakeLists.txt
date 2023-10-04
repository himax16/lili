# Create mesh library
add_library(mesh INTERFACE)
add_library(save_mesh STATIC save_mesh.cpp save_mesh.hpp)

# Include directories for mesh library
target_include_directories(mesh INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(save_mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(save_mesh PUBLIC mesh)

# Link external libraries
target_link_libraries(save_mesh PUBLIC hdf5::hdf5)