# Create track particle library
add_library(track_particle STATIC track_particle.hpp track_particle.cpp)

# Include directories for the library
target_include_directories(track_particle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(track_particle PUBLIC particle)