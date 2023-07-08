# Create input library
add_library(input STATIC input.cpp input.hpp)

# Include directories for input library
target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(input PUBLIC output)

# For json.hpp
target_link_libraries(input PUBLIC json)