/**
 * @file main.cpp
 * @brief Main LILI program
 *
 */

// #include <numbers>
#include <fstream>
#include <iostream>

#include "config.h"
#include "json.hpp"  // Header-only nlohmann/json library
// #include "parameter.h"

using json = nlohmann::json;

/**
 * @brief Print current `LILI` version with the current git SHA1 and status.
 *
 */
void print_version() {
  // Report version
  std::cout << PROJECT_NAME << " v" << PROJECT_VER << std::endl;
  std::cout << "git SHA1: " << PROJECT_GITHASH << " (" << PROJECT_GITSTATUS
            << ")" << std::endl;
}

/**
 * @brief Main `LILI` program
 *
 * @param argc
 *  Number of arguments to the program
 * @param argv
 *  Program run arguments
 * @return int
 *  Program return code
 */
int main(int argc, char *argv[]) {
  // int n_simsys = 0;
  // bool has_mhd = false;
  // bool use_emf = true;

  // int i_loop = 0;
  // int n_loop = 10;

  // Parse argument
  if (argc < 2) {
    print_version();
    std::cout << "Usage: " << argv[0] << " [input_file]" << std::endl;
    // return 0;
  }

  // // parse_input(input_file);

  // // Initialize simulation system based on the input file
  // if (n_simsys <= 0) {
  //   std::cerr << "Number of simulated system must be positive" << std::endl;
  //   exit(10);
  // }
  // for (int i_simsys = 0; i_simsys < n_simsys; ++i_simsys) {
  //   switch (simsys[i_simsys].type) {
  //     case 'm':
  //       if (has_mhd) {
  //         std::cerr << "There are multiple MHD simulation system" <<
  //         std::endl; exit(12);
  //       }
  //       initialize_mhdsys(simsys[i_simsys].property);
  //       has_mhd = true;
  //     case 'p':
  //       initialize_parsys(simsys[i_simsys].property);
  //       break;
  //     case 'f':
  //       initialize_flusys(simsys[i_simsys].property);
  //       break;
  //     case 'n':
  //       initialize_neusys(simsys[i_simsys].property);
  //       break;
  //     default:
  //       std::cerr << "Unrecognized simulation system" << std::endl;
  //       exit(11);
  //   }
  // }

  // // Initialize field variable if needed
  // if (use_emf) {
  //   initialize_emfsys();
  // }

  // // boost::json::object obj;
  // // obj["pi"] = boost::math::constants::pi<double>();

  // // std::cout << obj << std::endl;

  // // LILI main loop
  // for (i_loop = 0; i_loop < n_loop; ++i_loop) {
  //   std::cout << i_loop << std::endl;

  // }

  json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;
  // explicit conversion to string
  std::string s = j.dump();  // {"happy":true,"pi":3.141}

  // serialization with pretty printing
  // pass in the amount of spaces to indent
  std::cout << j.dump(4) << std::endl;

  std::cout << "LILI main program test" << std::endl;
  return (0);
}
