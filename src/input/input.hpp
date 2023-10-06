/**
 * @file input.hpp
 * @brief Header file for processing CLI and user inputs
 */
#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "mesh.hpp"

namespace lili::input {
// Input type
typedef enum {
  None,
  Initial,
  Restart,
} InputType;

// InputParticle struct
typedef struct {
  int n;
  double m;
  double q;
  double tau;
  std::string name;
} InputParticle;

// Input class
class Input {
 public:
  // Constructor
  Input();
  Input(const char* in_file);

  // Copy constructor
  Input(const Input& input);

  // Move constructor
  Input(Input&& input) noexcept : Input() { swap(*this, input); };

  // Destructor
  ~Input() = default;

  // Swap data
  friend void swap(Input& first, Input& second);

  // Operators
  Input& operator=(Input other) {
    swap(*this, other);
    return *this;
  }

  // Getters
  std::string input_file() const { return input_file_; }
  std::string problem_name() const { return problem_name_; }
  InputType input_type() const { return input_type_; }
  lili::mesh::MeshSize mesh() const { return mesh_; }
  std::vector<InputParticle> particles() const { return particles_; }

  // Setters
  std::string& input_file() { return input_file_; }
  std::string& problem_name() { return problem_name_; }
  InputType& input_type() { return input_type_; }
  lili::mesh::MeshSize& mesh() { return mesh_; }
  std::vector<InputParticle>& particles() { return particles_; }

  // Function to parse input file
  void Parse();

 private:
  std::string input_file_;
  std::string problem_name_;
  InputType input_type_;

  lili::mesh::MeshSize mesh_;
  std::vector<InputParticle> particles_;
};

// Function declaration
Input ParseArguments(int argc, char* argv[]);
}  // namespace lili::input