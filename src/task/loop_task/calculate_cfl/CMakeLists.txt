# Create calc_cfl library
add_library(calc_cfl STATIC calc_cfl.hpp calc_cfl.cpp)

# Include directories for task library
target_include_directories(calc_cfl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(calc_cfl PUBLIC task)
target_link_libraries(calc_cfl PUBLIC particle)

# Link external libraries
