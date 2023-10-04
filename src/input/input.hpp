/**
 * @file input.hpp
 * @brief Header file for processing CLI and user inputs
 */
#pragma once

#include <fstream>
#include <iostream>

namespace lili::input {
// Variable declaration
extern std::string gInputFile;
extern std::string gProblemName;
extern int gInputType;

// Function declaration
void ParseInput(char *in_file);
void ParseArguments(int argc, char *argv[]);
}  // namespace lili::input