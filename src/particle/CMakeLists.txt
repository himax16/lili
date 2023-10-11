# Create particle library
add_library(particle STATIC particle.hpp particle.cpp)

# Include directories for the library
target_include_directories(particle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link internal libraries
target_link_libraries(particle PUBLIC input)

# Add subdirectories
add_subdirectory(track_particle)