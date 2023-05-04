# Compiled macros and functions used for LILI CMake

################################################################################
# Macros
################################################################################

# Set the C++ standard
macro(set_lili_cxx)
  # C++ standard should be at least 2017
  if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
  endif()
  # C++ standard should be required
  if(NOT DEFINED CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
  endif()
  # C++ file exntension should be ignored
  if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
  endif()
endmacro()