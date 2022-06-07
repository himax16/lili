#ifndef INPUT_HPP_
#define INPUT_HPP_
//==============================================================================
// HIMA
//==============================================================================
// input.hpp
//
// Header file for reading the input files
//==============================================================================

// C headers

// C++ headers
#include <iostream>
#include <fstream>

// External libraries headers
#include <boost/json/src.hpp>

// Project headers
// #include "parameter.h"

// Declarations
namespace m::input{
void parse_arguments(int argc, char *argv[], boost::json::object parameters);
void parse_inputfile();
}

#endif // INPUT_HPP_
