#pragma once
/**
 * @file input.hpp
 * @brief Header file for reading the input files
 */

#include <fstream>
#include <iostream>

// External libraries headers
// #include <boost/json/src.hpp>

// Project headers
// #include "parameter.h"

// Declarations
namespace input {
class Input;
void parse_arguments(int argc, char *argv[]);
void parse_inputfile();
}  // namespace input
