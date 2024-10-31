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
#include "output.hpp"

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
  /**
   * @brief Default constructor for the Input class
   *
   * @details
   * Initialize the Input object with default values:
   * ```cpp
   * input_file = "";
   * problem_name = "LILI";
   * input_type = InputType::None;
   * ```
   */
  Input();

  /**
   * @brief Constructor for Input class for a given input file
   *
   * @param[in] in_file
   * Input file
   * @details
   * Initialize the Input object with the given input file:
   * ```cpp
   * input_file = in_file;
   * problem_name = "LILI";
   * input_type = InputType::None;
   * ```
   */
  Input(const char* in_file);

  /**
   * @brief Copy constructor for the Input class
   *
   * @param input Other Input object
   */
  Input(const Input& input);

  /**
   * @brief Move constructor for the Input class
   *
   * @param input Pointer to the other Input object
   */
  Input(Input&& input) noexcept : Input() { swap(*this, input); };

  // Destructor
  ~Input() = default;

  /**
   * @brief Function to swap the data between two Input objects
   *
   * @param first First Input object
   * @param second Second Input object
   * @details
   * This function will swap the data between two Input objects in-place using
   * `std::swap`.
   */
  friend void swap(Input& first, Input& second);

  // Operators
  /// @cond OPERATORS
  Input& operator=(Input other) {
    swap(*this, other);
    return *this;
  }
  /// @endcond

  // Getters
  /// @cond GETTERS
  std::string input_file() const { return input_file_; }
  std::string problem_name() const { return problem_name_; }
  std::string restart_file() const { return restart_file_; }
  InputType input_type() const { return input_type_; }
  lili::mesh::MeshSize mesh() const { return mesh_; }
  std::vector<InputParticle> particles() const { return particles_; }
  InputIntegrator integrator() const { return integrator_; }
  /// @endcond

  // Setters
  /// @cond SETTERS
  std::string& input_file() { return input_file_; }
  std::string& problem_name() { return problem_name_; }
  std::string& restart_file() { return restart_file_; }
  InputType& input_type() { return input_type_; }
  lili::mesh::MeshSize& mesh() { return mesh_; }
  std::vector<InputParticle>& particles() { return particles_; }
  InputIntegrator& integrator() { return integrator_; }
  /// @endcond

  /**
   * @brief Parse input file in the object and store the data
   *
   * @details
   * This function will parse the input file based on the filename stored in the
   * object. The parsed data will be stored in the object.
   *
   * The input file should be in JSON format.
   */
  void Parse();

  /**
   * @brief Print the input information
   *
   * @param lout Custom LiliCout class for output
   */
  void Print(lili::output::LiliCout& lout) {
    lout << "Input file   : " << input_file_ << std::endl;
    lout << "Problem name : " << problem_name_ << std::endl;
    lout << "Input type   : " << lili::input::InputTypeToString(input_type_)
         << std::endl;
    lout << "Mesh size    : " << std::endl;
    lili::mesh::PrintMeshSize(mesh_);
  }

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
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @param lout Custom LiliCout class for output
 * @return
 * Input object with parsed information
 */
Input ParseArguments(int argc, char** argv, lili::output::LiliCout& lout);
}  // namespace lili::input
