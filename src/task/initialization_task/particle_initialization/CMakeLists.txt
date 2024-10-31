# Create particle initialization library
add_library(
  task_par_init STATIC
  task_par_init.hpp task_par_init.cpp)

# Include directories for the library
target_include_directories(task_par_init PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(task_par_init PUBLIC task)
target_link_libraries(task_par_init PUBLIC particle)
target_link_libraries(task_par_init PUBLIC mesh)
