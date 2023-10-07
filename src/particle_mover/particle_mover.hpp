/**
 * @file particle_mover.hpp
 * @brief Header file for the ParticleMover class
 */
#pragma once

#include "field.hpp"
#include "particle.hpp"

namespace lili::particle {
/**
 * @brief Enumeration class for the particle mover type
 */
typedef enum : int {
  None,  /**< No particle mover */
  Boris, /**< Boris particle mover */
  Vay    /**< Vay particle mover */
} ParticleMoverType;

/**
 * @brief Base class for Particle mover
 */
class ParticleMover {
 public:
  // Constructor
  ParticleMover() : type_(ParticleMoverType::None), dt_(0.0), cache_(nullptr){};
  ParticleMover(double dt)
      : type_(ParticleMoverType::None), dt_(dt), cache_(nullptr){};

  // Destructor
  ~ParticleMover() {
    if (cache_ != nullptr) {
      delete[] cache_;
    }
  };

  // Cache initialization
  virtual void InitializeCache(Particles& particles, mesh::Field& field);

  // Move particles
  virtual void Move(Particles& particles, mesh::Field& field);

  // Getter
  constexpr ParticleMoverType type() const { return type_; };
  constexpr double dt() const { return dt_; };
  constexpr double* cache() const { return cache_; };

  // Setter
  constexpr ParticleMoverType& type() { return type_; };
  constexpr double& dt() { return dt_; };

 private:
  ParticleMoverType type_;
  double dt_;
  double* cache_;
};

/**
 * @brief Class for the Boris particle mover
 */
class BorisParticleMover : public ParticleMover {
 public:
  // Constructor
  BorisParticleMover() : ParticleMover() {
    ParticleMover::type() = ParticleMoverType::Boris;
  };
  BorisParticleMover(double dt) : ParticleMover(dt) {
    ParticleMover::type() = ParticleMoverType::Boris;
  };

  // Destructor
  ~BorisParticleMover(){};

  // Cache initialization
  void InitializeCache(Particles& particles, mesh::Field& field);

  // Move particles
  void Move(Particles& particles, mesh::Field& field);

  // Getter
  constexpr double qm() const { return qm_; };

  // Setter
  constexpr double& qm() { return qm_; };

 private:
  double qm_;
};

}  // namespace lili::particle