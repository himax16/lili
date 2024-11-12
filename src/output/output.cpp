/**
 * @file    output.cpp
 * @brief   Source file for processing CLI outputs
 */
#include "output.hpp"

#include <mpi.h>

namespace lili::output {
void PrintVersion() {
  // Report version
  std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;
  std::cout << "git SHA1: " << PROJECT_GITHASH;
  std::cout << " (" << PROJECT_GITSTATUS << ")" << std::endl;
}

void PrintVersion(LiliCout& out) {
  // Report version
  out << PROJECT_NAME << " v" << PROJECT_VER << std::endl;
  out << "git SHA1: " << PROJECT_GITHASH;
  out << " (" << PROJECT_GITSTATUS << ")" << std::endl;
}

void PrintHelp() {
  // Print help message
  std::cout << "Usage: lili [OPTION]... [INPUT FILE]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help     Show this help message" << std::endl;
  std::cout << "  -i, --input    Input file" << std::endl;
  std::cout << "  -v, --version  Output version information" << std::endl;
}

void PrintHelp(LiliCout& out) {
  // Print help message
  out << "Usage: lili [OPTION]... [INPUT FILE]" << std::endl;
  out << "Options:" << std::endl;
  out << "  -h, --help     Show this help message" << std::endl;
  out << "  -i, --input    Input file" << std::endl;
  out << "  -v, --version  Output version information" << std::endl;
}

void LiliExit(int status) {
  // Check if MPI is initialized
  int mpi_initialized;

  MPI_Initialized(&mpi_initialized);

  if (mpi_initialized) {
    // Finalize MPI
    MPI_Abort(MPI_COMM_WORLD, status);
  } else {
    // Exit the program
    exit(status);
  }
}
}  // namespace lili::output
