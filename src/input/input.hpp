#pragma once
/**
 * @file input.hpp
 * @brief Header file for processing CLI and user inputs
 */

#include <fstream>
#include <iostream>

#include "json.hpp"

// Simplify namespace
using json = nlohmann::json;

namespace lili::input {
// Input class
class Input {
 public:
  const std::string input_file;
  const std::string problem_name;
  const int input_type;

  Input(std::string input_file, std::string problem_name, int input_type)
      : input_file(input_file),
        problem_name(problem_name),
        input_type(input_type){};

  ~Input(){};
};

// Variable declaration
extern std::string input_file;
extern std::string problem_name;
extern int input_type;

// Function declaration
void print_version();
void print_help();
void parse_input(char *input_file);
void parse_arguments(int argc, char *argv[]);
}  // namespace lili::input