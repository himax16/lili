//==============================================================================
// HIMA
//==============================================================================
// main.cpp
//
// Main loop program
//==============================================================================

// C headers

// C++ headers
// #include <numbers>
#include <iostream>
#include <fstream>

// External libraries headers
#include <boost/json/src.hpp>
#include <boost/math/constants/constants.hpp>

// Project headers
#include "config.h"
// #include "parameter.h"

// Temporary
// -----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
  if (argc < 2) {
    // Report version
    std::cout << "Project " << PROJECT_NAME
              << " Version " << PROJECT_VER << std::endl;
    std::cout << "Git hash: " << PROJECT_GITHASH
              << " (" << PROJECT_GITSTATUS << ")" << std::endl;
    std::cout << "Usage: " << argv[0] << " [input_file]" << std::endl;
    return 1;
  }

  boost::json::object obj;
  obj["pi"] = boost::math::constants::pi<double>();

  std::cout << obj << std::endl;

  int i_loop = 0;
  int n_loop = 10;
  for (i_loop = 0; i_loop < n_loop; ++i_loop) {
    std::cout << i_loop << std::endl;
  }

  return(0);
}
