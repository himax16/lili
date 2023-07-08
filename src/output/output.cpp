/*
 * @file    output.cpp
 * @brief   Source file for processing CLI outputs
 */
#include "output.hpp"

/**
 * @brief Print current `LILI` version with the current git SHA1 and status.
 */
void lili::output::print_version() {
  // Report version
  std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;
  std::cout << "git SHA1: " << PROJECT_GITHASH;
  std::cout << " (" << PROJECT_GITSTATUS << ")" << std::endl;
}

/**
 * @brief Print help message
 */
void lili::output::print_help() {
  // Print help message
  std::cout << "Usage: lili [OPTION]... [INPUT FILE]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help     Show this help message" << std::endl;
  std::cout << "  -i, --input    Input file" << std::endl;
  std::cout << "  -v, --version  Output version information" << std::endl;
}
