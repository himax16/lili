/**
 * @file particle_mover.hpp
 * @brief Header file for the ParticleMover class
 */
#pragma once

#include "field.hpp"
#include "input.hpp"
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
  ParticleMover()
      : type_(ParticleMoverType::None),
        dt_(0.0),
        cache_(nullptr),
        Move_(nullptr){};

  // Destructor
  ~ParticleMover() {
    if (cache_ != nullptr) {
      delete[] cache_;
    }
  };

  // Initialize Mover
  virtual void InitializeMover(const input::Input& input);

  // Move particles
  void Move(Particles& particles, mesh::Field& field) {
    (this->*Move_)(particles, field);
  };

  // Getter
  constexpr ParticleMoverType type() const { return type_; };
  constexpr double dt() const { return dt_; };
  constexpr double* cache() const { return cache_; };

  // Setter
  constexpr ParticleMoverType& type() { return type_; };
  constexpr double& dt() { return dt_; };

 protected:
  ParticleMoverType type_;
  double dt_;
  double* cache_;

  // Function pointer to actual Mover used
  void (ParticleMover::*Move_)(Particles& particles, mesh::Field& field);
};

/**
 * @brief Class for the Boris particle mover
 */
class BorisParticleMover : public ParticleMover {
 public:
  // Constructor
  BorisParticleMover() : ParticleMover() { type_ = ParticleMoverType::Boris; };
  BorisParticleMover(input::Input input) : ParticleMover() {
    InitializeMover(input);
  };

  // Destructor
  ~BorisParticleMover(){};

  // Initialize Mover
  void InitializeMover(const input::Input& input);

  // Getter
  constexpr double qm() const { return qm_; };

  // Setter
  constexpr double& qm() { return qm_; };

 private:
  int dim_;
  double qm_;

  // Move particles
  void Move2D(Particles& particles, mesh::Field& field);
};

}  // namespace lili::particle