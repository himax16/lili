# Produce the main program in the project binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Make sure the build-time configured config.h is readable
include_directories("${PROJECT_BINARY_DIR}")

# Build the main program
add_executable(lili main.cpp)
target_include_directories(lili PUBLIC .)
target_link_libraries(lili PUBLIC json)
# target_include_directories(
#   main
#   PUBLIC ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS}
# )
