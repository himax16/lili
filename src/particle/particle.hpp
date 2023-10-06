/**
 * @file particle.hpp
 * @brief Header file for the Particles class
 */
#pragma once

#include <algorithm>
#include <cstdint>

#ifndef __LILIP_DEFAULT_BSIZE
#define __LILIP_DEFAULT_BSIZE 1000000
#endif

#ifndef __LILIP_DEFAULT_GSIZE
#define __LILIP_DEFAULT_GSIZE 2
#endif

#define __LILIP_DCOUNT_UINT32 2
#define __LILIP_DCOUNT_DOUBLE 6

namespace lili::particle {
extern const char* __LILIP_DNAME_UINT32[];
extern const char* __LILIP_DNAME_DOUBLE[];

/**
 * @brief Enumeration class for the particle status
 */
typedef enum : uint32_t {
  Out,     /**< Irrelevant status */
  In,      /**< Particle is in the simulation domain */
  Tracked, /**< Particle is tracked */
  X0,      /**< Particle crossed the negative X boundary */
  X1,      /**< Particle crossed the positive X boundary */
  Y0,      /**< Particle crossed the negative Y boundary */
  Y1,      /**< Particle crossed the positive Y boundary */
  Z0,      /**< Particle crossed the negative Z boundary */
  Z1,      /**< Particle crossed the positive Z boundary */
  TX0,     /**< Particle crossed the negative X boundary and is tracked */
  TX1,     /**< Particle crossed the positive X boundary and is tracked */
  TY0,     /**< Particle crossed the negative Y boundary and is tracked */
  TY1,     /**< Particle crossed the positive Y boundary and is tracked */
  TZ0,     /**< Particle crossed the negative Z boundary and is tracked */
  TZ1      /**< Particle crossed the positive Z boundary and is tracked */
} ParticleStatus;

class Particles {
 public:
  // Constructor
  Particles();
  Particles(uint32_t npar);
  Particles(uint32_t npar, uint32_t npar_max);

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
  constexpr uint32_t npar() const { return npar_; };
  constexpr uint32_t npar_max() const { return npar_max_; };

  constexpr uint32_t* id() const { return id_; };
  constexpr ParticleStatus* status() const { return status_; };
  constexpr double* x() const { return x_; };
  constexpr double* y() const { return y_; };
  constexpr double* z() const { return z_; };
  constexpr double* u() const { return u_; };
  constexpr double* v() const { return v_; };
  constexpr double* w() const { return w_; };

  constexpr uint32_t id(uint32_t i) const { return id_[i]; };
  constexpr ParticleStatus status(uint32_t i) const { return status_[i]; };
  constexpr double x(uint32_t i) const { return x_[i]; };
  constexpr double y(uint32_t i) const { return y_[i]; };
  constexpr double z(uint32_t i) const { return z_[i]; };
  constexpr double u(uint32_t i) const { return u_[i]; };
  constexpr double v(uint32_t i) const { return v_[i]; };
  constexpr double w(uint32_t i) const { return w_[i]; };

  // Setters
  constexpr uint32_t& npar() { return npar_; };

  constexpr uint32_t& id(uint32_t i) { return id_[i]; };
  constexpr ParticleStatus& status(uint32_t i) { return status_[i]; };
  constexpr double& x(uint32_t i) { return x_[i]; };
  constexpr double& y(uint32_t i) { return y_[i]; };
  constexpr double& z(uint32_t i) { return z_[i]; };
  constexpr double& u(uint32_t i) { return u_[i]; };
  constexpr double& v(uint32_t i) { return v_[i]; };
  constexpr double& w(uint32_t i) { return w_[i]; };

  // Data pointers for HDF5
  constexpr uint32_t* data_uint32(int i) {
    switch (i) {
      case 0:
        return id_;
      case 1:
        return reinterpret_cast<uint32_t*>(status_);
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
  void resize(uint32_t new_npar_max);

  // Add offset to particle IDs
  void AddID(uint32_t offset);

  // Swap two swap two particles
  void pswap(uint32_t i, uint32_t j);

  // Clean up out particles
  void CleanOut();

 private:
  uint32_t npar_, npar_max_;

  uint32_t* id_;
  ParticleStatus* status_;

  double *x_, *y_, *z_;
  double *u_, *v_, *w_;
};

void SaveParticles(Particles& particles, const char* file_name);
Particles LoadParticles(const char* file_name);

void SelectParticles(Particles& input, Particles& output, ParticleStatus status,
                     bool remove = false);

}  // namespace lili::particle
