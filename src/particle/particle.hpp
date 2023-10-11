/**
 * @file particle.hpp
 * @brief Header file for the Particles class
 */
#pragma once

#include <algorithm>
#include <cstdint>

#include "input.hpp"

#ifndef __LILIP_DEFAULT_BSIZE
#define __LILIP_DEFAULT_BSIZE 1000000
#endif

#ifndef __LILIP_DEFAULT_GSIZE
#define __LILIP_DEFAULT_GSIZE 2
#endif

#define __LILIP_DCOUNT_ULONG 2
#define __LILIP_DCOUNT_DOUBLE 6

namespace lili::particle {
extern const char* __LILIP_DNAME_UINT32[];
extern const char* __LILIP_DNAME_DOUBLE[];
extern const char* __LILIP_SNAME[];

/**
 * @brief Enumeration class for the particle status
 */
typedef enum {
  Out,     /**< Irrelevant status */
  In,      /**< Particle is in the simulation domain */
  Tracked, /**< Particle is tracked */
  X0,      /**< Particle crossed the negative X boundary */
  X1,      /**< Particle crossed the positive X boundary */
  Y0,      /**< Particle crossed the negative Y boundary */
  Y1,      /**< Particle crossed the positive Y boundary */
  Z0,      /**< Particle crossed the negative Z boundary */
  Z1,      /**< Particle crossed the positive Z boundary */
  X0Y0,    /**< Particle crossed the negative X and Y boundaries */
  X0Y1,    /**< Particle crossed the negative X and positive Y boundaries */
  X1Y0,    /**< Particle crossed the positive X and negative Y boundaries */
  X1Y1,    /**< Particle crossed the positive X and Y boundaries */
  X0Z0,    /**< Particle crossed the negative X and Z boundaries */
  X0Z1,    /**< Particle crossed the negative X and positive Z boundaries */
  X1Z0,    /**< Particle crossed the positive X and negative Z boundaries */
  X1Z1,    /**< Particle crossed the positive X and Z boundaries */
  Y0Z0,    /**< Particle crossed the negative Y and Z boundaries */
  Y0Z1,    /**< Particle crossed the negative Y and positive Z boundaries */
  Y1Z0,    /**< Particle crossed the positive Y and negative Z boundaries */
  Y1Z1,    /**< Particle crossed the positive Y and Z boundaries */
  X0Y0Z0,  /**< Particle crossed the negative X, Y and Z boundaries */
  X0Y0Z1,  /**< Particle crossed the negative X, Y and positive Z boundaries */
  X0Y1Z0,  /**< Particle crossed the negative X, positive Y and negative Z
              boundaries */
  X0Y1Z1,  /**< Particle crossed the negative X, positive Y and Z boundaries */
  X1Y0Z0,  /**< Particle crossed the positive X, negative Y and negative Z
              boundaries */
  X1Y0Z1,  /**< Particle crossed the positive X, negative Y and Z boundaries */
  X1Y1Z0,  /**< Particle crossed the positive X, Y and negative Z boundaries */
  X1Y1Z1,  /**< Particle crossed the positive X, Y and Z boundaries */
  TX0,     /**< Particle crossed the negative X boundary and is tracked */
  TX1,     /**< Particle crossed the positive X boundary and is tracked */
  TY0,     /**< Particle crossed the negative Y boundary and is tracked */
  TY1,     /**< Particle crossed the positive Y boundary and is tracked */
  TZ0,     /**< Particle crossed the negative Z boundary and is tracked */
  TZ1,     /**< Particle crossed the positive Z boundary and is tracked */
  TX0Y0,   /**< Particle crossed the negative X and Y boundaries and is
              tracked */
  TX0Y1,   /**< Particle crossed the negative X and positive Y boundaries and
              is tracked */
  TX1Y0,   /**< Particle crossed the positive X and negative Y boundaries and
              is tracked */
  TX1Y1,   /**< Particle crossed the positive X and Y boundaries and is
              tracked */
  TX0Z0,   /**< Particle crossed the negative X and Z boundaries and is
              tracked */
  TX0Z1,   /**< Particle crossed the negative X and positive Z boundaries and
              is tracked */
  TX1Z0,   /**< Particle crossed the positive X and negative Z boundaries and
              is tracked */
  TX1Z1,   /**< Particle crossed the positive X and Z boundaries and is
                tracked */
  TY0Z0,   /**< Particle crossed the negative Y and Z boundaries and is
              tracked */
  TY0Z1,   /**< Particle crossed the negative Y and positive Z boundaries and
              is tracked */
  TY1Z0,   /**< Particle crossed the positive Y and negative Z boundaries and
              is tracked */
  TY1Z1,   /**< Particle crossed the positive Y and Z boundaries and is
              tracked */
  TX0Y0Z0, /**< Particle crossed the negative X, Y and Z boundaries and is
              tracked */
  TX0Y0Z1, /**< Particle crossed the negative X, Y and positive Z boundaries
              and is tracked */
  TX0Y1Z0, /**< Particle crossed the negative X, positive Y and negative Z
              boundaries and is tracked */
  TX0Y1Z1, /**< Particle crossed the negative X, positive Y and Z boundaries
              and is tracked */
  TX1Y0Z0, /**< Particle crossed the positive X, negative Y and negative Z
              boundaries and is tracked */
  TX1Y0Z1, /**< Particle crossed the positive X, negative Y and Z boundaries
              and is tracked */
  TX1Y1Z0, /**< Particle crossed the positive X, Y and negative Z boundaries
              and is tracked */
  TX1Y1Z1  /**< Particle crossed the positive X, Y and Z boundaries and is
               tracked */
} ParticleStatus;

class Particles {
 public:
  // Constructor
  Particles();
  Particles(int npar);
  Particles(int npar, int npar_max);
  Particles(input::InputParticle input_particle);

  // Copy constructor
  Particles(const Particles& other);

  // Move constructor
  Particles(Particles&& other) noexcept : Particles() { swap(*this, other); };

  // Destructor
  ~Particles();

  // Swap data
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
  Particles& operator=(Particles other) {
    swap(*this, other);
    return *this;
  }

  // Getters
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

  // Setters
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

  // Data pointers for HDF5
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

  // Resize particles
  void resize(int new_npar_max);

  // Add offset to particle IDs
  void AddID(int offset);

  // Swap two swap two particles
  void pswap(const int i, const int j);

  // Clean up out particles
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

}  // namespace lili::particle
