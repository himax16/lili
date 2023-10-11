/**
 * @file track_particle.hpp
 * @brief Header file for the TrackParticle helper class for tracking particles
 */
#pragma once

#include "particle.hpp"

namespace lili::particle {
/**
 * @brief TrackParticle class
 */
class TrackParticle {
 public:
  // Constructor
  TrackParticle(int ntrack, int ndump)
      : ntrack_(ntrack), ndump_(ndump), itrack_(0), idump_(0) {
    InitializeTrackParticle();
  }

  // Destructor
  ~TrackParticle() = default;

  // Initialize the TrackParticle class
  void InitializeTrackParticle();

  // Save tracked particles
  void SaveTrackedParticles(Particles &particles);

  // Dump tracked particles
  void DumpTrackedParticles();

  // Public data members
  Particles track_particles;

 private:
  int ntrack_, ndump_, itrack_, idump_;
  ulong *idtrack_;
  double *xtrack_, *ytrack_, *ztrack_;
  double *utrack_, *vtrack_, *wtrack_;
};
}  // namespace lili::particle