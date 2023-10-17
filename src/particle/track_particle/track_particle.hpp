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
  TrackParticles()
      : ntrack_(0), ndump_(0), itrack_(0), idump_(0), prefix_("tp_") {
    InitializeTrackParticles();
  }
  TrackParticles(int ntrack, int ndump)
      : ntrack_(ntrack), ndump_(ndump), itrack_(0), idump_(0), prefix_("tp_") {
    InitializeTrackParticles();
  }

  // Copy constructor
  TrackParticles(const TrackParticles &other)
      : ntrack_(other.ntrack_),
        ndump_(other.ndump_),
        itrack_(other.itrack_),
        idump_(other.idump_),
        prefix_(other.prefix_) {
    InitializeTrackParticles();
    std::copy(other.idtrack_, other.idtrack_ + ntrack_ * ndump_, idtrack_);
    std::copy(other.xtrack_, other.xtrack_ + ntrack_ * ndump_, xtrack_);
    std::copy(other.ytrack_, other.ytrack_ + ntrack_ * ndump_, ytrack_);
    std::copy(other.ztrack_, other.ztrack_ + ntrack_ * ndump_, ztrack_);
    std::copy(other.utrack_, other.utrack_ + ntrack_ * ndump_, utrack_);
    std::copy(other.vtrack_, other.vtrack_ + ntrack_ * ndump_, vtrack_);
    std::copy(other.wtrack_, other.wtrack_ + ntrack_ * ndump_, wtrack_);
  }

  // Move constructor
  TrackParticles(TrackParticles &&other) noexcept : TrackParticles() {
    swap(*this, other);
  };

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

  // Swap data
  friend void swap(TrackParticles &first, TrackParticles &second) noexcept {
    using std::swap;
    swap(first.ntrack_, second.ntrack_);
    swap(first.ndump_, second.ndump_);
    swap(first.itrack_, second.itrack_);
    swap(first.idump_, second.idump_);
    swap(first.prefix_, second.prefix_);

    swap(first.idtrack_, second.idtrack_);
    swap(first.xtrack_, second.xtrack_);
    swap(first.ytrack_, second.ytrack_);
    swap(first.ztrack_, second.ztrack_);
    swap(first.utrack_, second.utrack_);
    swap(first.vtrack_, second.vtrack_);
    swap(first.wtrack_, second.wtrack_);
  }

  // Operators
  TrackParticles &operator=(TrackParticles other) {
    swap(*this, other);
    return *this;
  }

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