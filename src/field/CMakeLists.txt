# Create field library
add_library(field STATIC field.hpp field.cpp)

# Include directories for the library
target_include_directories(field PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(field PUBLIC mesh)