# Create task library
add_library(task STATIC task.hpp task.cpp)

# Include directories for task library
target_include_directories(task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(task PUBLIC input)

# Link external libraries
