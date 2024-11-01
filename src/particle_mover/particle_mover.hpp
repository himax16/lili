/**
 * @file particle_mover.hpp
 * @brief Header file for the ParticleMover class
 */
#pragma once

#include "fields.hpp"
#include "input.hpp"
#include "particle.hpp"

namespace lili::particle {
/**
 * @brief Enumeration class for the particle mover type
 */
typedef enum : int {
  None,     ///< No particle mover */
  Boris2D,  ///< Boris 2D particle mover */
  Boris3D   ///< Boris 3D particle mover */
} ParticleMoverType;

/**
 * @brief Class for Particle mover
 */
class ParticleMover {
 public:
  // Constructor
  ParticleMover()
      : type_(ParticleMoverType::None),
        dt_(1.0),
        cache_(nullptr),
        Move_(nullptr) {};

  // Destructor
  ~ParticleMover() {
    if (cache_ != nullptr) {
      delete[] cache_;
    }
  };

  // Initialize Mover
  void InitializeMover(const input::InputLoop& input);

  // Move particles
  void Move(Particles& particles, const mesh::Fields& fields) {
    (this->*Move_)(particles, fields);
  };

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

  // Function pointer to actual Mover used
  void (ParticleMover::*Move_)(Particles& particles,
                               const mesh::Fields& fields);

  // Different Movers
  void MoveNone(Particles& particles, const mesh::Fields& fields) {
    std::cout << "Moving particles using no particle mover" << std::endl;

    int npar = particles.npar();
    double* __restrict__ ex = fields.ex.data();
    double sum = 0.;
    for (int i = 0; i < npar; ++i) {
      sum += ex[i];
    }
  };
  void MoveBoris2D(Particles& particles, const mesh::Fields& fields);
};

}  // namespace lili::particle
