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

  # C++ file extension should be ignored
  if(NOT DEFINED CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
  endif()
endmacro()

# Set the MPI library
macro(set_lili_mpi)
  message(STATUS "Setting MPI")
  
  # Find MPI
  find_package(MPI REQUIRED)
endmacro()

# Set the OpenMP library
macro(set_lili_openmp)
  message(STATUS "Setting OpenMP")
  
  # Find OpenMP
  find_package(OpenMP REQUIRED)
endmacro()

# Set the HDF5 library
# Careful that FindHDF5.cmake API is not consistent across versions
macro(set_lili_hdf5)
  message(STATUS "Setting HDF5")
  
  # Set HDF5 library type
  set(HDF5_USE_STATIC_LIBRARIES ON)

  # Skip looking for hdf5-config.cmake as most clusters don't have it
  set(HDF5_NO_FIND_PACKAGE_CONFIG_FILE TRUE)

  # Print more information for loading HDF5
  set(HDF5_FIND_DEBUG TRUE)

  # Find HDF5
  find_package(HDF5 REQUIRED COMPONENTS C)
endmacro()

# Set the compiler flags
macro(set_lili_compiler_flags)
  message(STATUS "Setting compiler flags")

  # Set the compiler flags
  add_compile_options(
    -Wall -Wextra -Werror
    -O3
    # -g -fsanitize=address
  )

  # Set the linker flags
  # set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
endmacro()