/**
 * @file particle.hpp
 * @brief Header file for the Particles class
 */
#pragma once

#include <algorithm>
#include <cstdint>

#include "input.hpp"

#ifndef __LILIP_DEFAULT_BSIZE
/**
 * @brief Default buffer size for the Particles class
 */
#define __LILIP_DEFAULT_BSIZE 1000000
#endif

#ifndef __LILIP_DEFAULT_GSIZE
/**
 * @brief Default grow factor for the Particles class
 */
#define __LILIP_DEFAULT_GSIZE 2
#endif
/**
 * @brief Number of unsigned long data in the Particles class
 */
#define __LILIP_DCOUNT_ULONG 2
/**
 * @brief Number of double data in the Particles class
 */
#define __LILIP_DCOUNT_DOUBLE 6

/**
 * @brief Namespace for LILI particle related routines
 */
namespace lili::particle {
// Global variables
/**
 * @brief Default names for the unsigned long data
 */
extern const char* __LILIP_DNAME_UINT32[];
/**
 * @brief Default names for the double data
 */
extern const char* __LILIP_DNAME_DOUBLE[];

/**
 * @brief Enumeration class for the particle status
 *
 * @details
 * This enumeration class is useful to flag the particle and can be extended
 * further. Currently it tracks whether the particle is tracked or not and
 * whether it has crossed the boundary or not.
 */
enum class ParticleStatus {
  Out,      ///< Out of domain, to be removed
  In,       ///< Inside the domain
  Tracked,  ///< Tracked, inside the domain
  X0,       ///< Crossed the -X boundary
  X1,       ///< Crossed the +X boundary
  Y0,       ///< Crossed the -Y boundary
  Y1,       ///< Crossed the +Y boundary
  Z0,       ///< Crossed the -Z boundary
  Z1,       ///< Crossed the +Z boundary
  X0Y0,     ///< Crossed the -X and -Y boundary
  X0Y1,     ///< Crossed the -X and +Y boundary
  X1Y0,     ///< Crossed the +X and -Y boundary
  X1Y1,     ///< Crossed the +X and +Y boundary
  X0Z0,     ///< Crossed the -X and -Z boundary
  X0Z1,     ///< Crossed the -X and +Z boundary
  X1Z0,     ///< Crossed the +X and -Z boundary
  X1Z1,     ///< Crossed the +X and +Z boundary
  Y0Z0,     ///< Crossed the -Y and -Z boundary
  Y0Z1,     ///< Crossed the -Y and +Z boundary
  Y1Z0,     ///< Crossed the +Y and -Z boundary
  Y1Z1,     ///< Crossed the +Y and +Z boundary
  X0Y0Z0,   ///< Crossed the -X, -Y, and -Z boundary
  X0Y0Z1,   ///< Crossed the -X, -Y, and +Z boundary
  X0Y1Z0,   ///< Crossed the -X, +Y, and -Z boundary
  X0Y1Z1,   ///< Crossed the -X, +Y, and +Z boundary
  X1Y0Z0,   ///< Crossed the +X, -Y, and -Z boundary
  X1Y0Z1,   ///< Crossed the +X, -Y, and +Z boundary
  X1Y1Z0,   ///< Crossed the +X, +Y, and -Z boundary
  X1Y1Z1,   ///< Crossed the +X, +Y, and +Z boundary
  TX0,      ///< Tracked, crossed the -X boundary
  TX1,      ///< Tracked, crossed the +X boundary
  TY0,      ///< Tracked, crossed the -Y boundary
  TY1,      ///< Tracked, crossed the +Y boundary
  TZ0,      ///< Tracked, crossed the -Z boundary
  TZ1,      ///< Tracked, crossed the +Z boundary
  TX0Y0,    ///< Tracked, crossed the -X and -Y boundary
  TX0Y1,    ///< Tracked, crossed the -X and +Y boundary
  TX1Y0,    ///< Tracked, crossed the +X and -Y boundary
  TX1Y1,    ///< Tracked, crossed the +X and +Y boundary
  TX0Z0,    ///< Tracked, crossed the -X and -Z boundary
  TX0Z1,    ///< Tracked, crossed the -X and +Z boundary
  TX1Z0,    ///< Tracked, crossed the +X and -Z boundary
  TX1Z1,    ///< Tracked, crossed the +X and +Z boundary
  TY0Z0,    ///< Tracked, crossed the -Y and -Z boundary
  TY0Z1,    ///< Tracked, crossed the -Y and +Z boundary
  TY1Z0,    ///< Tracked, crossed the +Y and -Z boundary
  TY1Z1,    ///< Tracked, crossed the +Y and +Z boundary
  TX0Y0Z0,  ///< Tracked, crossed the -X, -Y, and -Z boundary
  TX0Y0Z1,  ///< Tracked, crossed the -X, -Y, and +Z boundary
  TX0Y1Z0,  ///< Tracked, crossed the -X, +Y, and -Z boundary
  TX0Y1Z1,  ///< Tracked, crossed the -X, +Y, and +Z boundary
  TX1Y0Z0,  ///< Tracked, crossed the +X, -Y, and -Z boundary
  TX1Y0Z1,  ///< Tracked, crossed the +X, -Y, and +Z boundary
  TX1Y1Z0,  ///< Tracked, crossed the +X, +Y, and -Z boundary
  TX1Y1Z1   ///< Tracked, crossed the +X, +Y, and +Z boundary
};

/**
 * @brief Class to store particles data of a single species
 */
class Particles {
 public:
  // Constructor
  Particles();
  Particles(int npar);
  Particles(int npar, int npar_max);
  Particles(input::InputParticles input_particle);

  // Copy constructor
  Particles(const Particles& other);

  // Move constructor
  Particles(Particles&& other) noexcept : Particles() { swap(*this, other); };

  // Destructor
  ~Particles();

  /**
   * @brief Function to swap the data between two Particles objects
   *
   * @param first First Particles object
   * @param second Second Particles object
   * @details
   * This function will swap the data between two Particles objects in-place
   * using std::swap.
   */
  friend void swap(Particles& first, Particles& second) noexcept {
    using std::swap;
    swap(first.npar_, second.npar_);
    swap(first.npar_max_, second.npar_max_);

    swap(first.q_, second.q_);
    swap(first.m_, second.m_);

    swap(first.id_, second.id_);
    swap(first.status_, second.status_);

    swap(first.x_, second.x_);
    swap(first.y_, second.y_);
    swap(first.z_, second.z_);
    swap(first.u_, second.u_);
    swap(first.v_, second.v_);
    swap(first.w_, second.w_);
  }

  // Operators
  /// @cond OPERATORS
  Particles& operator=(Particles other) {
    swap(*this, other);
    return *this;
  }
  /// @endcond

  // Getters
  /// @cond GETTERS
  constexpr int npar() const { return npar_; };
  constexpr int npar_max() const { return npar_max_; };

  constexpr double q() const { return q_; };
  constexpr double m() const { return m_; };

  constexpr ulong* id() const { return id_; };
  constexpr ParticleStatus* status() const { return status_; };
  constexpr double* x() const { return x_; };
  constexpr double* y() const { return y_; };
  constexpr double* z() const { return z_; };
  constexpr double* u() const { return u_; };
  constexpr double* v() const { return v_; };
  constexpr double* w() const { return w_; };

  constexpr ulong id(int i) const { return id_[i]; };
  constexpr ParticleStatus status(int i) const { return status_[i]; };
  constexpr double x(int i) const { return x_[i]; };
  constexpr double y(int i) const { return y_[i]; };
  constexpr double z(int i) const { return z_[i]; };
  constexpr double u(int i) const { return u_[i]; };
  constexpr double v(int i) const { return v_[i]; };
  constexpr double w(int i) const { return w_[i]; };
  /// @endcond

  // Setters
  /// @cond SETTERS
  constexpr int& npar() { return npar_; };

  constexpr double& q() { return q_; };
  constexpr double& m() { return m_; };

  constexpr ulong& id(int i) { return id_[i]; };
  constexpr ParticleStatus& status(int i) { return status_[i]; };
  constexpr double& x(int i) { return x_[i]; };
  constexpr double& y(int i) { return y_[i]; };
  constexpr double& z(int i) { return z_[i]; };
  constexpr double& u(int i) { return u_[i]; };
  constexpr double& v(int i) { return v_[i]; };
  constexpr double& w(int i) { return w_[i]; };
  /// @endcond

  // Data pointers for HDF5
  /**
   * @brief Helper function to iterate over ulong data arrays for HDF5 output
   *
   * @param i Index of the data array
   * @return constexpr ulong* Pointer to the data array
   * @details
   * Currently the index `i` represents the following data arrays:
   * | `i` | Data array | Description |
   * | :-: | :-: | :- |
   * | 0 | `id` | Particle ID |
   * | 1 | `status` | Particle status |
   */
  constexpr ulong* data_uint32(int i) {
    switch (i) {
      case 0:
        return id_;
      case 1:
        return reinterpret_cast<ulong*>(status_);
      default:
        return nullptr;
    }
  };

  /**
   * @brief Helper function to iterate over double data arrays for HDF5 output
   *
   * @param i Index of the data array
   * @return constexpr double* Pointer to the data array
   * @details
   * Currently the index `i` represents the following data arrays:
   * | `i` | Data array | Description |
   * | :-: | :-: | :- |
   * | 0 | `x` | Particle \f$x_i\f$ coordinate |
   * | 1 | `y` | Particle \f$y_i\f$ coordinate |
   * | 2 | `z` | Particle \f$z_i\f$ coordinate |
   * | 3 | `u` | Particle \f$v_{x, i}\f$ velocity |
   * | 4 | `v` | Particle \f$v_{y, i}\f$ velocity |
   * | 5 | `w` | Particle \f$v_{z, i}\f$ velocity |
   */
  constexpr double* data_double(int i) {
    switch (i) {
      case 0:
        return x_;
      case 1:
        return y_;
      case 2:
        return z_;
      case 3:
        return u_;
      case 4:
        return v_;
      case 5:
        return w_;
      default:
        return nullptr;
    }
  };

  /**
   * @brief Resize the size of data arrays
   *
   * @param new_npar_max New maximum number of particles
   */
  void resize(int new_npar_max);

  /**
   * @brief Add an integer offset to the particle ID
   *
   * @param offset Offset to be added
   */
  void AddID(int offset);

  /**
   * @brief Swap two particles data
   *
   * @param i Index of the first particle
   * @param j Index of the second particle
   */
  void pswap(const int i, const int j);

  /**
   * @brief Function to clean up particles that are outside of the domain with
   * ParticleStatus::Out status.
   */
  void CleanOut();

 private:
  int npar_, npar_max_;
  double q_, m_;

  ulong* id_;
  ParticleStatus* status_;

  double *x_, *y_, *z_;
  double *u_, *v_, *w_;
};

void SaveParticles(Particles& particles, const char* file_name);
Particles LoadParticles(const char* file_name);

void SelectParticles(Particles& input, Particles& output, ParticleStatus status,
                     bool remove = false);
void LabelBoundaryParticles(Particles& particles, mesh::MeshSize mesh_size);
void PeriodicBoundaryParticles(Particles& particles, mesh::MeshSize mesh_size);

}  // namespace lili::particle
