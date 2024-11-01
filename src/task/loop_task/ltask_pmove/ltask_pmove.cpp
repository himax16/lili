/**
 * @file ltask_pmove.cpp
 * @brief Source file for the ParticleMover class
 */
#include "ltask_pmove.hpp"

#include <cmath>

#include "parameter.hpp"

namespace lili::particle {
/**
 * @brief Initialize ParticleMover
 * @param[in] input
 * Input object
 */
void ParticleMover::InitializeMover(const input::InputLoop& input) {
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
 * @param[in] fields
 * Fields object
 */
void ParticleMover::MoveBoris2D(Particles& particles,
                                const mesh::Fields& fields) {
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

  const double crx = fields.size.nx / fields.size.lx;
  const double cry = fields.size.ny / fields.size.ly;

  // Loop over the particles
  for (int i = 0; i < npar; ++i) {
    // Get the particle position
    rx = (x[i] - fields.size.x0) * crx;
    ry = (y[i] - fields.size.y0) * cry;

    ex = qmhdt * fields.ex.BilinearInterpolation(rx, ry);
    ey = qmhdt * fields.ey.BilinearInterpolation(rx, ry);
    ez = qmhdt * fields.ez.BilinearInterpolation(rx, ry);

    bx = qmhdt * fields.bx.BilinearInterpolation(rx, ry);
    by = qmhdt * fields.by.BilinearInterpolation(rx, ry);
    bz = qmhdt * fields.bz.BilinearInterpolation(rx, ry);

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

namespace lili::task {
void TaskMoveParticlesFull::Initialize() {
  // Get the particles and fields from the simulation variables
  particles_ptr_ = std::get<std::unique_ptr<std::vector<particle::Particles>>>(
                       sim_vars[SimVarType::ParticlesVector])
                       .get();
  fields_ptr_ =
      std::get<std::unique_ptr<mesh::Fields>>(sim_vars[SimVarType::EMFields])
          .get();
}
void TaskMoveParticlesFull::Execute() {
  // Loop through all species
  for (auto& particles : *particles_ptr_) {
    // Move particles
    (mover_.Move)(particles, *fields_ptr_);
  }

  // Increment the run counter
  IncrementRun();
}
}  // namespace lili::task
