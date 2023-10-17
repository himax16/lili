/**
 * @file track_particle.hpp
 * @brief Header file for the TrackParticles helper class for tracking particles
 */
#pragma once

#include <string>

#include "particle.hpp"

namespace lili::particle {
/**
 * @brief TrackParticles class
 */
class TrackParticles {
 public:
  // Constructor
  TrackParticles(int ntrack, int ndump)
      : ntrack_(ntrack), ndump_(ndump), itrack_(0), idump_(0), prefix_("tp_") {
    InitializeTrackParticles();
  }

  // Destructor
  ~TrackParticles() {
    delete[] idtrack_;
    delete[] xtrack_;
    delete[] ytrack_;
    delete[] ztrack_;
    delete[] utrack_;
    delete[] vtrack_;
    delete[] wtrack_;
  };

  // Initialize the TrackParticles class
  void InitializeTrackParticles();

  // Save tracked particles
  void SaveTrackedParticles(Particles &particles);

  // Dump tracked particles
  void DumpTrackedParticles();

  // Getter
  int ntrack() const { return ntrack_; }
  int ndump() const { return ndump_; }
  int itrack() const { return itrack_; }
  int idump() const { return idump_; }
  std::string prefix() const { return prefix_; }

  // Setter
  void SetPrefix(std::string prefix) { prefix_ = prefix; }

  // Public data members
  Particles track_particles;

 private:
  int ntrack_, ndump_, itrack_, idump_;
  std::string prefix_;
  ulong *idtrack_;
  double *xtrack_, *ytrack_, *ztrack_;
  double *utrack_, *vtrack_, *wtrack_;
};
}  // namespace lili::particle