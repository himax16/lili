# Create ltask_cfl library
add_library(ltask_cfl STATIC ltask_cfl.hpp ltask_cfl.cpp)

# Include directories for task library
target_include_directories(ltask_cfl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(ltask_cfl PUBLIC task)
target_link_libraries(ltask_cfl PUBLIC particle)

# Link external libraries
