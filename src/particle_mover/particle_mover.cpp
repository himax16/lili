/**
 * @file particle_mover.cpp
 * @brief Source file for the ParticleMover class
 */
#include "particle_mover.hpp"

namespace lili::particle {
/**
 * @brief Initialize cache for the Boris particle mover
 *
 * @param[in] particles
 * Particles object
 * @param[in] field
 * Field object
 * @details
 * This function initializes the cache for the Boris particle mover that will
 * contain the interpolated electromagnetic field at the particle location.
 * Is currently not implemented.
 */
void BorisParticleMover::InitializeCache(Particles& particles,
                                         mesh::Field& field) {}
/**
 * @brief Move particles using the Boris particle mover
 *
 * @param[in] particles
 * Particles object
 * @param[in] field
 * Field object
 */
void BorisParticleMover::Move(Particles& particles, mesh::Field& field) {
  // Get the number of particles
  int npar = particles.npar();
}
}  // namespace lili::particle