# Create particle mover library
add_library(particle_mover STATIC particle_mover.cpp particle_mover.hpp)

# Include directories for the library
target_include_directories(particle_mover PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(particle_mover PUBLIC particle input mesh fields)
