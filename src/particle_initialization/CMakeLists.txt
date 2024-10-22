# Create particle initialization library
add_library(
  particle_initialization STATIC
  particle_initialization.hpp particle_initialization.cpp)

# Include directories for the library
target_include_directories(particle_initialization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(particle_initialization PUBLIC particle)
target_link_libraries(particle_initialization PUBLIC mesh)
