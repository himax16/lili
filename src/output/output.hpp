/**
 * @file output.hpp
 * @brief Header file for processing CLI outputs
 */
#pragma once

#include <iostream>

#include "config.h"

/**
 * @brief Rate at which the loop prints the iteration information
 */
#ifndef __LILI_LPRINT_RATE
#define __LILI_LPRINT_RATE 10000
#endif

/**
 * @brief Namespace for output related functions
 */
namespace lili::output {
/**
 * @brief Custom `std::cout` class to enable/disable printing output
 */
class LiliCout {
 public:
  LiliCout() : enabled(true) {}
  LiliCout(bool enabled) : enabled(enabled) {}

  /**
   * @brief Extraction operator for LiliCout class similar to `std::cout`
   *
   * @tparam T Type of the input
   * @param t Input to be printed
   * @return LiliCout& Reference to the LiliCout object
   */
  template <typename T>
  LiliCout& operator<<(const T& t) {
    if (enabled) std::cout << t;
    return *this;
  }

  /**
   * @brief Extraction operator for LiliCout class similar to `std::cout`
   *
   * @param f Function pointer to `std::ostream&`
   * @return LiliCout& Reference to the LiliCout object
   */
  LiliCout& operator<<(std::ostream& (*f)(std::ostream&)) {
    if (enabled) std::cout << f;
    return *this;
  }

  /**
   * @brief Flag to enable/disable printing output
   */
  bool enabled;
};

// Function declaration
/**
 * @brief Print current `LILI` version with the current git SHA1 and status.
 */
void PrintVersion();
void PrintVersion(LiliCout& out);

/**
 * @brief Print the help message for the project
 */
void PrintHelp();
void PrintHelp(LiliCout& out);

/**
 * @brief Custom wrapper for exit function to handle MPI finalize
 * @param status Exit status
 * @details
 * This function will finalize MPI before exiting the program.
 *
 * @todo Add a custom exit for compiling without MPI.
 */
void LiliExit(int status);
}  // namespace lili::output
