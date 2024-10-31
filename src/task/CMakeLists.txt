# Create task library
add_library(task STATIC task.hpp task.cpp)

# Include directories for task library
target_include_directories(task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(task PUBLIC parameter)
target_link_libraries(task PUBLIC input)
target_link_libraries(task PUBLIC fields)
target_link_libraries(task PUBLIC particle)
target_link_libraries(task PUBLIC track_particle)

# Link external libraries
