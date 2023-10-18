/**
 * @file particle_mover.cpp
 * @brief Source file for the ParticleMover class
 */
#include "particle_mover.hpp"

#include <cmath>

namespace lili::particle {
/**
 * @brief Initialize ParticleMover
 * @param[in] input
 * Input object
 */
void ParticleMover::InitializeMover(const input::InputIntegrator& input) {
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

  // Set the time step
  dt_ = input.dt;
}

/**
 * @brief Move particles using the Boris particle mover
 *
 * @param[in] particles
 * Particles object
 * @param[in] field
 * Field object
 */
void ParticleMover::MoveBoris2D(Particles& particles,
                                const mesh::Field& field) {
  // Initialize variables
  const int npar = particles.npar();
  const double qmhdt = particles.q() * dt_ / (2.0 * particles.m());

  // Get the particle information
  double* __restrict__ x = particles.x();
  double* __restrict__ y = particles.y();
  double* __restrict__ z = particles.z();

  double* __restrict__ u = particles.u();
  double* __restrict__ v = particles.v();
  double* __restrict__ w = particles.w();

  double rx, ry;
  double ex, ey, ez, bx, by, bz;
  double um, vm, wm, up, vp, wp;
  double temp;

  const double crx = field.size.nx / field.size.lx;
  const double cry = field.size.ny / field.size.ly;

  // Loop over the particles
  for (int i = 0; i < npar; ++i) {
    // Get the particle position
    rx = (x[i] - field.size.x0) * crx;
    ry = (y[i] - field.size.y0) * cry;

    ex = qmhdt * field.ex.BilinearInterpolation(rx, ry);
    ey = qmhdt * field.ey.BilinearInterpolation(rx, ry);
    ez = qmhdt * field.ez.BilinearInterpolation(rx, ry);

    bx = qmhdt * field.bx.BilinearInterpolation(rx, ry);
    by = qmhdt * field.by.BilinearInterpolation(rx, ry);
    bz = qmhdt * field.bz.BilinearInterpolation(rx, ry);

    // First half acceleration
    um = u[i] + ex;
    vm = v[i] + ey;
    wm = w[i] + ez;

    // First half of the rotation
    temp = 1.0 / std::sqrt(1.0 + um * um + vm * vm + wm * wm);
    bx *= temp;
    by *= temp;
    bz *= temp;

    temp = 2.0 / (1.0 + bx * bx + by * by + bz * bz);
    up = (um + vm * bz - wm * by) * temp;
    vp = (vm + wm * bx - um * bz) * temp;
    wp = (wm + um * by - vm * bx) * temp;

    // Second half acceleration
    um = um + ex + vp * bz - wp * by;
    vm = vm + ey + wp * bx - up * bz;
    wm = wm + ez + up * by - vp * bx;

    // Advance position
    temp = 1.0 / std::sqrt(1.0 + um * um + vm * vm + wm * wm);
    x[i] += dt_ * um * temp;
    y[i] += dt_ * vm * temp;
    z[i] += dt_ * wm * temp;

    // Update velocity
    u[i] = um;
    v[i] = vm;
    w[i] = wm;
  }
}
}  // namespace lili::particle