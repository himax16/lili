# Create fields library
add_library(fields STATIC fields.hpp fields.cpp)

# Include directories for the library
target_include_directories(fields PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(fields PUBLIC mesh)
