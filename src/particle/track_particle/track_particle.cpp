/**
 * @file track_particle.cpp
 * @brief Source file for the TrackParticle helper class for tracking particles
 */

#include "track_particle.hpp"

#include <iostream>

namespace lili::particle {
/**
 * @brief Initialize the TrackParticle class
 */
void TrackParticle::InitializeTrackParticle() {
  // Initialize the particles
  track_particles = Particles(ntrack_);

  // Allocate memory for the tracked particles
  idtrack_ = new ulong[ntrack_ * ndump_]();
  xtrack_ = new double[ntrack_ * ndump_]();
  ytrack_ = new double[ntrack_ * ndump_]();
  ztrack_ = new double[ntrack_ * ndump_]();
  utrack_ = new double[ntrack_ * ndump_]();
  vtrack_ = new double[ntrack_ * ndump_]();
  wtrack_ = new double[ntrack_ * ndump_]();
}

/**
 * @brief Save tracked particles
 */
void TrackParticle::SaveTrackedParticles(Particles &particles) {
  // Copy tracked particles to the current cache
  SelectParticles(particles, track_particles, ParticleStatus::Tracked);
  if (track_particles.npar() != ntrack_) {
    std::cout << "Error: number of tracked particles is not correct"
              << std::endl;
    exit(1);
  }

  // Move the data to the dump cache
  for (int i_track = 0; i_track < ntrack_; ++i_track) {
    idtrack_[idump_ * ntrack_ + i_track] = track_particles.id()[i_track];
    xtrack_[idump_ * ntrack_ + i_track] = track_particles.x()[i_track];
    ytrack_[idump_ * ntrack_ + i_track] = track_particles.y()[i_track];
    ztrack_[idump_ * ntrack_ + i_track] = track_particles.z()[i_track];
    utrack_[idump_ * ntrack_ + i_track] = track_particles.u()[i_track];
    vtrack_[idump_ * ntrack_ + i_track] = track_particles.v()[i_track];
    wtrack_[idump_ * ntrack_ + i_track] = track_particles.w()[i_track];
  }

  // Increment the dump index
  ++idump_;

  // Dump the tracked particles if the dump index reaches the dump number
  if (idump_ >= ndump_) {
    DumpTrackedParticles();
    idump_ = 0;
  }
}

/**
 * @brief Dump tracked particles
 */
void TrackParticle::DumpTrackedParticles() {
  // Dump the tracked particles
  std::cout << "Dumping tracked particles..." << std::endl;
  for (int i_dump = 0; i_dump < ndump_; ++i_dump) {
    std::cout << "Dump " << i_dump << std::endl;
    for (int i_track = 0; i_track < ntrack_; ++i_track) {
      std::cout << idtrack_[i_dump * ntrack_ + i_track] << " "
                << xtrack_[i_dump * ntrack_ + i_track] << " "
                << ytrack_[i_dump * ntrack_ + i_track] << " "
                << ztrack_[i_dump * ntrack_ + i_track] << " "
                << utrack_[i_dump * ntrack_ + i_track] << " "
                << vtrack_[i_dump * ntrack_ + i_track] << " "
                << wtrack_[i_dump * ntrack_ + i_track] << std::endl;
    }
  }
}
}  // namespace lili::particle