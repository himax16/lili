# Create task library
add_library(task STATIC task.hpp task.cpp)

# Include directories for task library
target_include_directories(task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add subdirectories
add_subdirectory(initialization_task)
add_subdirectory(loop_task)

# Link internal libraries
target_link_libraries(task PUBLIC parameter)
target_link_libraries(task PUBLIC input)
target_link_libraries(task PUBLIC fields)
target_link_libraries(task PUBLIC particle)
target_link_libraries(task PUBLIC track_particle)

# Link other task libraries
target_link_libraries(task PUBLIC task_init_fields)
target_link_libraries(task PUBLIC task_init_particles)

# Link external libraries
