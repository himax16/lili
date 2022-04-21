# Produce the main program in the project binary directory
message(STATUS "print: ${GIT_SHA1}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
add_executable(main main.cpp)
target_include_directories(main PUBLIC ${PROJECT_BINARY_DIR})
