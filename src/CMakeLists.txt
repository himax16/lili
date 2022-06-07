# Produce the main program in the project binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
add_executable(main main.cpp)
target_include_directories(
  main
  PUBLIC ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS}
)
