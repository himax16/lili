/**
 * @file particle_mover.cpp
 * @brief Source file for the ParticleMover class
 */
#include "particle_mover.hpp"

namespace lili::particle {
/**
 * @brief Initialize ParticleMover
 * @param[in] input
 * Input object
 */
void ParticleMover::InitializeMover(const input::Input&) {
  // Set the particle mover type
  type_ = ParticleMoverType::Boris2D;

  // Set the Mover function pointer
  switch (type_) {
    case ParticleMoverType::Boris2D:
      Move_ = &ParticleMover::MoveBoris2D;
      break;

    default:
      Move_ = &ParticleMover::MoveNone;
      break;
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
void ParticleMover::MoveBoris2D(Particles& particles, mesh::Field& field) {
  std::cout << "Moving particles using Boris particle mover" << std::endl;
  // Initialize variables
  int npar = particles.npar();

  // Get the particle information
  double* __restrict__ x = particles.x();
  double* __restrict__ y = particles.y();
  double* __restrict__ z = particles.z();

  double* __restrict__ u = particles.u();
  double* __restrict__ v = particles.v();
  double* __restrict__ w = particles.w();

  // Debug
  std::cout << "x = " << x[0] << std::endl;
  std::cout << "y = " << y[0] << std::endl;
  std::cout << "z = " << z[0] << std::endl;

  std::cout << "u = " << u[0] << std::endl;
  std::cout << "v = " << v[0] << std::endl;
  std::cout << "w = " << w[0] << std::endl;

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