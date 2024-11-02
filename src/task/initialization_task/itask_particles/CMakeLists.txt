# Create particle initialization library
add_library(
  itask_particles STATIC
  itask_particles.hpp itask_particles.cpp)

# Include directories for the library
target_include_directories(itask_particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(itask_particles PUBLIC task)
target_link_libraries(itask_particles PUBLIC particle)
target_link_libraries(itask_particles PUBLIC mesh)
