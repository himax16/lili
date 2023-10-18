# Produce the main program in the project binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Add library directories
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(mesh)
add_subdirectory(field)
add_subdirectory(particle)
add_subdirectory(particle_initialization)
add_subdirectory(particle_mover)

# # Add parameter.hpp to an interface library
# add_library(parameter INTERFACE)
# target_include_directories(parameter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add the main program
add_executable(lili lili.cpp)

# Add the libraries to the main program
target_link_libraries(lili PUBLIC input)
target_link_libraries(lili PUBLIC output)
target_link_libraries(lili PUBLIC mesh)
target_link_libraries(lili PUBLIC field)
target_link_libraries(lili PUBLIC particle)
target_link_libraries(lili PUBLIC track_particle)
target_link_libraries(lili PUBLIC particle_initialization)
target_link_libraries(lili PUBLIC particle_mover)
target_link_libraries(lili PUBLIC MPI::MPI_C)
target_link_libraries(lili PUBLIC hdf5::hdf5)
target_link_libraries(lili PUBLIC stdc++fs)

# HIMA: Kalau pakai Boost
# target_include_directories(
#   main
#   PUBLIC ${PROJECT_BINARY_DIR} ${Boost_INCLUDE_DIRS}
# )
