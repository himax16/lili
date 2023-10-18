/**
 * @file input.hpp
 * @brief Header file for processing CLI and user inputs
 */
#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "mesh.hpp"

namespace lili::input {
/**
 * @brief Enumeration class for input type
 */
typedef enum {
  None,         ///< No input type
  Initial,      ///< Initial input
  Restart,      ///< Restart input
  TestParticle  ///< Test particle input
} InputType;

/**
 * @brief Function to convert InputType to string
 *
 * @param[in] input_type
 * InputType to be converted
 */
std::string InputTypeToString(InputType input_type);

/**
 * @brief Struct to store particle information from input file
 */
typedef struct {
  int n;
  int n_track;
  int dl_track;
  int dtrack_save;
  double q;
  double m;
  double tau;
  std::string name;
} InputParticle;

/**
 * @brief Struct to store integrator information from input file
 */
typedef struct {
  int n_loop;
  double dt;
} InputIntegrator;

/**
 * @brief Class to store input information
 */
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
  std::string restart_file() const { return restart_file_; }
  InputType input_type() const { return input_type_; }
  lili::mesh::MeshSize mesh() const { return mesh_; }
  std::vector<InputParticle> particles() const { return particles_; }
  InputIntegrator integrator() const { return integrator_; }

  // Setters
  std::string& input_file() { return input_file_; }
  std::string& problem_name() { return problem_name_; }
  std::string& restart_file() { return restart_file_; }
  InputType& input_type() { return input_type_; }
  lili::mesh::MeshSize& mesh() { return mesh_; }
  std::vector<InputParticle>& particles() { return particles_; }
  InputIntegrator& integrator() { return integrator_; }

  // Function to parse input file
  void Parse();

 private:
  std::string input_file_;
  std::string problem_name_;
  std::string restart_file_;

  InputType input_type_;

  lili::mesh::MeshSize mesh_;
  std::vector<InputParticle> particles_;
  InputIntegrator integrator_;
};

// Function declaration
Input ParseArguments(int argc, char* argv[]);

}  // namespace lili::input