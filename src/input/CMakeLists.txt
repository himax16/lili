
# Create input library
add_library(input STATIC input.cpp input.hpp)

# For config.h
# Make sure the build-time configured config.h is readable
include_directories("${PROJECT_BINARY_DIR}")
target_include_directories(input PRIVATE "${PROJECT_BINARY_DIR}")

# For json.hpp
target_link_libraries(input PUBLIC json)