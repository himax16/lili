# Create particle mover library
add_library(ltask_pmove STATIC ltask_pmove.cpp ltask_pmove.hpp)

# Include directories for the library
target_include_directories(ltask_pmove PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(ltask_pmove PUBLIC particle input mesh fields)
