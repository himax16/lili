# Produce the main program in the project binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Add library directories
add_subdirectory(input)

# # Add parameter.hpp to an interface library
# add_library(parameter INTERFACE)
# target_include_directories(parameter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Build the main program
add_executable(lili lili.cpp)
target_include_directories(lili PUBLIC input)
target_link_libraries(lili PUBLIC input)

# HIMA: Kalau pakai Boost
# target_include_directories(
#   main
#   PUBLIC ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS}
# )
