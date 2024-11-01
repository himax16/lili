# Create particle initialization library
add_library(
  task_init_fields STATIC
  task_init_fields.hpp task_init_fields.cpp)

# Include directories for the library
target_include_directories(task_init_fields PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(task_init_fields PUBLIC mesh)
target_link_libraries(task_init_fields PUBLIC input)
target_link_libraries(task_init_fields PUBLIC task)
target_link_libraries(task_init_fields PUBLIC fields)
