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

// Forward declaration
namespace input {
// Variable declaration
extern std::string input_file;
extern std::string problem_name;
extern int input_type;

// Function declaration
void print_version();
void print_help();
void parse_input(char *input_file);
void parse_arguments(int argc, char *argv[]);
}  // namespace input
