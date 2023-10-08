/**
 * @file particle_mover.cpp
 * @brief Source file for the ParticleMover class
 */
#include "particle_mover.hpp"

namespace lili::particle {
/**
 * @brief Initialize cache for the Boris particle mover
 * @param[in] input
 * Input object
 */
void BorisParticleMover::InitializeMover(const input::Input& input) {
  // Set the particle mover type
  type_ = ParticleMoverType::Boris;

  // Set the mesh dimension
  dim_ = input.mesh().dim;

  // Set the Mover function pointer
  if (dim_ == 2) {
    Move_ = &BorisParticleMover::Move2D;
  } else {
    std::cout << "ERROR: Invalid mesh dimension" << std::endl;
    exit(1);
  }
}

/**
 * @brief Move particles using the Boris particle mover
 *
 * @param[in] particles
 * Particles object
 * @param[in] field
 * Field object
 */
void BorisParticleMover::Move2D(Particles& particles, mesh::Field& field) {
  // Initialize variables
  int npar;

  // Get the number of particles
  npar = particles.npar();

  // Get the Field information
  double* __restrict__ ex = field.ex.data();

  double sum = 0.;

  // Loop over the particles
  for (int i = 0; i < npar; ++i) {
    sum += ex[i];
    // Get the particle position and velocity
    // double x = particles.x(i);
    // double y = particles.y(i);
    // double z = particles.z(i);

    // double u = particles.u(i);
    // double v = particles.v(i);
    // double w = particles.w(i);

    // // Get the electric field
    // double ex = field.ex(x, y, z);
    // double ey = field.ey(x, y, z);
    // double ez = field.ez(x, y, z);
  }
}
}  // namespace lili::particle