/**
 * @file itask_particles.hpp
 * @brief Header file for particle initialization routines
 */
#pragma once

#include <vector>

#include "mesh.hpp"
#include "particle.hpp"
#include "task.hpp"

#ifndef __LILIP_DEFAULT_GTSIZE
/**
 * @brief Default gamma table size
 */
#define __LILIP_DEFAULT_GTSIZE 10000
#endif

#ifndef __LILIP_DEFAULT_GTMULT
/**
 * @brief Default multiplier for the maximum delta gamma
 */
#define __LILIP_DEFAULT_GTMULT 20
#endif

namespace lili::particle {
/**
 * @brief Distribute particle IDs
 *
 * @param[in] particles Particles object
 * @param[in] offset Offset for the particle IDs
 */
void DistributeID(Particles& particles, const int offset);

/**
 * @brief Distribute particles uniformly in the simulation domain
 * @param[in] particles Particles object
 * @param[in] seed Seed for the random number generator
 * @param[in] x0 Minimum value of the X coordinate
 * @param[in] x1 Maximum value of the X coordinate
 * @param[in] y0 Minimum value of the Y coordinate
 * @param[in] y1 Maximum value of the Y coordinate
 * @param[in] z0 Minimum value of the Z coordinate
 * @param[in] z1 Maximum value of the Z coordinate
 */
void DistributeLocationUniform(Particles& particles, const int seed,
                               const double x0, const double x1,
                               const double y0, const double y1,
                               const double z0, const double z1);

/**
 * @brief Distribute particles uniformly in the simulation domain
 * @param[in] particles Particle object
 * @param[in] seed Seed for the random number generator
 * @param[in] domain_size MeshSize object of the domain
 */
void DistributeLocationUniform(Particles& particles, const int seed,
                               const mesh::MeshSize& domain_size);

/**
 * @brief Particle distribution functions for relativistic particles
 *
 * @details
 * This class stores a pair of vectors containing the cumulative distribution
 * function (CDF) and the corresponding particle gamma values.
 */
class GammaTable {
 public:
  // Constructor
  /**
   * @brief Base constructor for the GammaTable class
   */
  GammaTable();

  /**
   * @brief Constructor for given CDF and gamma values vectors
   *
   * @param cdf CDF values
   * @param gamma Gamma values
   */
  GammaTable(std::vector<double> cdf, std::vector<double> gamma);

  // Getters
  /// @cond GETTERS
  bool interpolate() const { return interpolate_; }
  int capacity() const { return capacity_; }
  double cdf(const int i) const { return cdf_[i]; }
  double gamma(const int i) const { return gamma_[i]; }
  /// @endcond

  // Setters
  /// @cond SETTERS
  bool& interpolate() { return interpolate_; }
  /// @endcond

  /**
   * @brief Function to get the gamma value from a given random number based on
   * the CDF table
   *
   * @param cdf Random number between 0 and 1
   * @return double Gamma value
   */
  double GetGamma(const double cdf) const;

 private:
  bool interpolate_;           ///< Interpolation flag
  int capacity_;               ///< Capacity of the table
  std::vector<double> cdf_;    ///< Cumulative distribution function
  std::vector<double> gamma_;  ///< Gamma values
};

// Gamma table initialization
/**
 * @brief Construct a new GammaTable for monoenergetic particles with \f$\gamma
 * = 1 + \delta \gamma\f$
 *
 * @param[in] delta_gamma \f$\delta \gamma\f$ value
 * @return GammaTable Particle gamma table
 */
GammaTable GTMonoenergetic(const double delta_gamma);

/**
 * @brief Construct a new Gamma Table for particles with uniform energy between
 * \f$\gamma_{\min}\f$ and \f$\gamma_{\max}\f$
 *
 * @param[in] gamma_min Minimum gamma value
 * @param[in] gamma_max Maximum gamma value
 * @return GammaTable Particle gamma table
 */
GammaTable GTUniform(const double gamma_min, const double gamma_max);

/**
 * @brief Construct a new Gamma Table for particles with 3D Maxwellian energy
 * distribution
 *
 * @param[in] theta Temperature parameter \f$\theta = \frac{k_B T}{m c^2}\f$
 * @return GammaTable Particle gamma table
 *
 * @details
 * Create a table of gamma values for particles with a Maxwellian energy
 * satisfying isotropic Maxwell-Juttner distribution:
 * @f[
 * f(\gamma) \sim \gamma (\gamma^2 - 1)^{\frac{d}{2} - 1}  e^{-\gamma/\theta}
 * @f]
 * where \f$ d \f$ is the number of spatial dimensions and \f$ \theta \f$ is the
 * temperature parameter.
 */
GammaTable GTMaxwellian3D(const double theta);

// Velocity initialization
/**
 * @brief Add bulk velocity \f$\mathbf{v}_0\f$ to the particles
 *
 * @param[in] particles Particles object
 * @param[in] u Bulk velocity in the X direction \f$v_{0,x}\f$
 * @param[in] v Bulk velocity in the Y direction \f$v_{0,y}\f$
 * @param[in] w Bulk velocity in the Z direction \f$v_{0,z}\f$
 */
void AddBulkVelocity(Particles& particles, const double u, const double v,
                     const double w);

/**
 * @brief Distribute particle velocities based on the given distribution
 * function
 *
 * @param[in] particles Particles object
 * @param[in] seed Seed for the random number generator
 * @param[in] energy_table Gamma table
 */
void DistributeVelocityUniform(Particles& particles, const int seed,
                               GammaTable& energy_table);

}  // namespace lili::particle

namespace lili::task {
/**
 * @brief Task class to initialize particles
 */
class TaskInitParticles : public Task {
 public:
  // Constructor
  TaskInitParticles() : Task(TaskType::InitParticles) {
    set_name("InitParticles");

    // Initialize the input particles
    input_particles_ = {};
  }
  TaskInitParticles(const input::Input input) : Task(TaskType::InitParticles) {
    set_name("InitParticles");

    // Copy the input.particles()
    input_particles_ = input.particles();
  }

  /**
   * @brief Initialize particles
   */
  void Execute() override;

  // Getters
  /// @cond GETTERS
  std::vector<particle::Particles>& particles() { return *particles_ptr_; }
  std::vector<particle::TrackParticles>& track_particles() {
    return *track_particles_ptr_;
  }

 private:
  int n_kind_;  ///< Number of particle species
  std::vector<input::InputParticles> input_particles_;  ///< Input particles

  /**
   * @brief Pointer to the simulation Particles vector
   */
  std::vector<particle::Particles>* particles_ptr_;
  /**
   * @brief Pointer to the simulation TrackParticles vector
   */
  std::vector<particle::TrackParticles>* track_particles_ptr_;
};
}  // namespace lili::task
