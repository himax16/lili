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
enum class InputType {
  None,         ///< No input type
  Initial,      ///< Initial input
  Restart,      ///< Restart input
  TestParticle  ///< Test particle input
};

/**
 * @brief Enumeration for particle position distribution function
 */
enum class PPosDist {
  Stationary,  ///< Stationary distribution
  Uniform      ///< Uniform distribution
};

/**
 * @brief Enumeration for particle velocity distribution function
 */
enum class PVelDist {
  Maxwellian  ///< Maxwellian distribution
};

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
 * @brief Simple class to store Particle input information
 */
class InputParticles {
 public:
  // Constructor
  /**
   * @brief Default constructor for the InputParticles class
   */
  InputParticles() {
    n = 0;
    n_track = 0;
    dl_track = 0;
    dtrack_save = 0;
    q = 0.;
    m = 0.;
    name = "";

    pos_dist = PPosDist::Stationary;
    pos_dist_param = {};

    vel_dist = PVelDist::Maxwellian;
    vel_dist_param = {};
    vel_offset = {0., 0., 0.};
  }

  /**
   * @brief Print the input particle information
   */
  void Print();

  std::string name;  ///< Particle name

  int n;            ///< Total number of particles
  double q;         ///< Particle charge \f$q_s\f$
  double m;         ///< Particle mass \f$m_s\f$

  int n_track;      ///< Total number of particles to track
  int dl_track;     ///< Number of time steps between tracking output
  int dtrack_save;  ///< Number of tracking outputs between saving

  PPosDist pos_dist;  ///< Particle position distribution
  std::vector<double>
      pos_dist_param;  ///< Particle position distribution parameters
  PVelDist vel_dist;   ///< Particle velocity distribution
  std::vector<double>
      vel_dist_param;  ///< Particle velocity distribution parameters
  std::vector<double> vel_offset;  ///< Particle velocity offset
};

/**
 * @brief Simple class to store loop tasks information
 */
class InputLoopTask {
 public:
  // Constructor
  /**
   * @brief Default constructor for the InputLoopTask class
   */
  InputLoopTask() {
    name = "";
    type = "";
  }

  std::string name;  ///< Task name
  std::string type;  ///< Task type
};

/**
 * @brief Simple class to store loop input information
 */
class InputLoop {
 public:
  // Constructor
  /**
   * @brief Default constructor for the InputLoop class
   */
  InputLoop() {
    n_loop = 0;
    dt = 0.;
    tasks = {};
  }

  int n_loop;  ///< Number of loop time steps \f$N_{\mathrm{loop}}\f$
  double dt;   ///< Loop time step \f$\mathrm{d}t\f$
  std::vector<InputLoopTask> tasks;  ///< List of loop tasks
};

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
  std::vector<InputParticles> particles() const { return particles_; }
  InputLoop loop() const { return loop_; }
  /// @endcond

  // Setters
  /// @cond SETTERS
  std::string& input_file() { return input_file_; }
  std::string& problem_name() { return problem_name_; }
  std::string& restart_file() { return restart_file_; }
  InputType& input_type() { return input_type_; }
  lili::mesh::MeshSize& mesh() { return mesh_; }
  std::vector<InputParticles>& particles() { return particles_; }
  InputLoop& loop() { return loop_; }
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
    lili::mesh::PrintMeshSize(mesh_, lout);
  }

 private:
  std::string input_file_;
  std::string problem_name_;
  std::string restart_file_;

  InputType input_type_;

  lili::mesh::MeshSize mesh_;
  std::vector<InputParticles> particles_;
  InputLoop loop_;
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
