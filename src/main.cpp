//==============================================================================
// HIMA
//==============================================================================
// main.cpp
//
// Main loop program
//==============================================================================

// C headers

// C++ headers
#include <iostream>

// External libraries headers

// Project headers
#include "config.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    // Report version
    std::cout << "Project " << PROJECT_NAME
              << " Version " << PROJECT_VER << std::endl;
    std::cout << "Git hash: " << PROJECT_GITHASH
              << " (" << PROJECT_GITSTATUS << ")" << std::endl;
    return 1;
  }

  return(0);
}
