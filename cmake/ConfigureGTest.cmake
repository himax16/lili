# Enable GoogleTest
message(STATUS "Enabling GoogleTest")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
mark_as_advanced(BUILD_GMOCK)
mark_as_advanced(INSTALL_GTEST)
FetchContent_MakeAvailable(googletest)