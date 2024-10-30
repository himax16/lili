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

/**
 * @brief
 * Namespace for LILI input processing routines
 */
namespace lili::input {
/**
 * @brief Enumeration for input type
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
 * @return
 * String representation of the InputType
 */
std::string InputTypeToString(InputType input_type);

/**
 * @brief Struct to store Particle input information
 */
typedef struct {
  int n;            ///< Total number of particles
  int n_track;      ///< Total number of particles to track
  int dl_track;     ///< Number of time steps between tracking output
  int dtrack_save;  ///< Number of tracking outputs between saving
  double q;         ///< Particle unit charge \f$q_0\f$
  double m;         ///< Particle unit mass \f$m_0\f$
  double tau;       ///< Particle temperature \f$\tau = \frac{k_B T}{m_0 c^2}\f$
  std::string name;  ///< Particle name
} InputParticle;

/**
 * @brief Struct to store Integrator input information
 */
typedef struct {
  int n_loop;  ///< Number of integrator time steps \f$N_{\mathrm{loop}}\f$
  double dt;   ///< Integrator time step \f$\mathrm{d}t\f$
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
/**
 * @brief Parse command line arguments and return an Input object
 *
 * @param[in] argc
 * Number of command line arguments
 * @param[in] argv
 * Command line arguments
 * @return
 * Input object with parsed information
 */
Input ParseArguments(int argc, char** argv);
}  // namespace lili::input
