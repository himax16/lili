/**
 * @file track_particle.hpp
 * @brief Header file for the TrackParticles helper class for tracking particles
 */
#pragma once

#include <string>

#include "field.hpp"
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
    std::copy(other.extrack_, other.extrack_ + ntrack_ * ndump_, extrack_);
    std::copy(other.eytrack_, other.eytrack_ + ntrack_ * ndump_, eytrack_);
    std::copy(other.eztrack_, other.eztrack_ + ntrack_ * ndump_, eztrack_);
    std::copy(other.bxtrack_, other.bxtrack_ + ntrack_ * ndump_, bxtrack_);
    std::copy(other.bytrack_, other.bytrack_ + ntrack_ * ndump_, bytrack_);
    std::copy(other.bztrack_, other.bztrack_ + ntrack_ * ndump_, bztrack_);
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
    delete[] extrack_;
    delete[] eytrack_;
    delete[] eztrack_;
    delete[] bxtrack_;
    delete[] bytrack_;
    delete[] bztrack_;
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
    swap(first.extrack_, second.extrack_);
    swap(first.eytrack_, second.eytrack_);
    swap(first.eztrack_, second.eztrack_);
    swap(first.bxtrack_, second.bxtrack_);
    swap(first.bytrack_, second.bytrack_);
    swap(first.bztrack_, second.bztrack_);
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
  void SaveTrackedParticles(Particles &particles, mesh::Field &field);

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
  double *extrack_, *eytrack_, *eztrack_;
  double *bxtrack_, *bytrack_, *bztrack_;
};
}  // namespace lili::particle