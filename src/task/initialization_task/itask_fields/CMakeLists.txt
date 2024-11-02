# Create particle initialization library
add_library(
  itask_fields STATIC
  itask_fields.hpp itask_fields.cpp)

# Include directories for the library
target_include_directories(itask_fields PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(itask_fields PUBLIC mesh)
target_link_libraries(itask_fields PUBLIC input)
target_link_libraries(itask_fields PUBLIC task)
target_link_libraries(itask_fields PUBLIC fields)
