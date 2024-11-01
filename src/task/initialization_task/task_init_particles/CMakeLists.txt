# Create particle initialization library
add_library(
  task_init_particles STATIC
  task_init_particles.hpp task_init_particles.cpp)

# Include directories for the library
target_include_directories(task_init_particles PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(task_init_particles PUBLIC task)
target_link_libraries(task_init_particles PUBLIC particle)
target_link_libraries(task_init_particles PUBLIC mesh)
