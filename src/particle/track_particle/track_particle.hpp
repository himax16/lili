/**
 * @file track_particle.hpp
 * @brief Header file for the TrackParticles helper class for tracking particles
 */
#pragma once

#include <string>

#include "fields.hpp"
#include "particle.hpp"

namespace lili::particle {
/**
 * @brief TrackParticles class
 */
class TrackParticles {
 public:
  // Constructor
  /**
   * @brief Default constructor for TrackParticles
   */
  TrackParticles()
      : n_track_(0), dtrack_save_(0), i_track_(0), i_dump_(0), prefix_("tp_") {
    InitializeTrackParticles();
  }

  /**
   * @brief Constructor for TrackParticles for given number of tracked particles
   * and number of time steps between tracking output
   *
   * @param n_track Number of tracked particles in the buffer
   * @param dtrack_save Number of time steps between tracking output
   */
  TrackParticles(int n_track, int dtrack_save)
      : n_track_(n_track),
        dtrack_save_(dtrack_save),
        i_track_(0),
        i_dump_(0),
        prefix_("tp_") {
    InitializeTrackParticles();
  }

  // Copy constructor
  TrackParticles(const TrackParticles& other)
      : n_track_(other.n_track_),
        dtrack_save_(other.dtrack_save_),
        i_track_(other.i_track_),
        i_dump_(other.i_dump_),
        prefix_(other.prefix_) {
    InitializeTrackParticles();
    std::copy(other.idtrack_, other.idtrack_ + n_track_ * dtrack_save_,
              idtrack_);
    std::copy(other.xtrack_, other.xtrack_ + n_track_ * dtrack_save_, xtrack_);
    std::copy(other.ytrack_, other.ytrack_ + n_track_ * dtrack_save_, ytrack_);
    std::copy(other.ztrack_, other.ztrack_ + n_track_ * dtrack_save_, ztrack_);
    std::copy(other.utrack_, other.utrack_ + n_track_ * dtrack_save_, utrack_);
    std::copy(other.vtrack_, other.vtrack_ + n_track_ * dtrack_save_, vtrack_);
    std::copy(other.wtrack_, other.wtrack_ + n_track_ * dtrack_save_, wtrack_);
    std::copy(other.extrack_, other.extrack_ + n_track_ * dtrack_save_,
              extrack_);
    std::copy(other.eytrack_, other.eytrack_ + n_track_ * dtrack_save_,
              eytrack_);
    std::copy(other.eztrack_, other.eztrack_ + n_track_ * dtrack_save_,
              eztrack_);
    std::copy(other.bxtrack_, other.bxtrack_ + n_track_ * dtrack_save_,
              bxtrack_);
    std::copy(other.bytrack_, other.bytrack_ + n_track_ * dtrack_save_,
              bytrack_);
    std::copy(other.bztrack_, other.bztrack_ + n_track_ * dtrack_save_,
              bztrack_);
  }

  // Move constructor
  TrackParticles(TrackParticles&& other) noexcept : TrackParticles() {
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
  friend void swap(TrackParticles& first, TrackParticles& second) noexcept {
    using std::swap;
    swap(first.n_track_, second.n_track_);
    swap(first.dtrack_save_, second.dtrack_save_);
    swap(first.i_track_, second.i_track_);
    swap(first.i_dump_, second.i_dump_);
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
  TrackParticles& operator=(TrackParticles other) {
    swap(*this, other);
    return *this;
  }

  /**
   * @brief Initialize the TrackParticles class
   */
  void InitializeTrackParticles();

  /**
   * @brief Save tracked particles with no fields information
   *
   * @param particles Particles object
   */
  void SaveTrackedParticles(Particles& particles);

  /**
   * @brief Save tracked particles with fields information
   *
   * @param particles Particles object
   * @param fields Fields object
   */
  void SaveTrackedParticles(Particles& particles, mesh::Fields& fields);

  /**
   * @brief Dump tracked particles to an HDF5 file
   */
  void DumpTrackedParticles();

  // Getter
  /// @cond GETTERS
  int n_track() const { return n_track_; }
  int dl_track() const { return dl_track_; }
  int dtrack_save() const { return dtrack_save_; }
  int i_track() const { return i_track_; }
  int i_dump() const { return i_dump_; }
  std::string prefix() const { return prefix_; }
  /// @endcond

  // Setter
  /// @cond SETTERS
  int& dl_track() { return dl_track_; }
  void SetPrefix(std::string prefix) { prefix_ = prefix; }
  /// @endcond

  // Public data members
  Particles track_particles;

 private:
  int n_track_;         ///< Number of tracked particles in the buffer
  int dl_track_;        ///< Number of loop iteration between tracking output
  int dtrack_save_;     ///< Number of tracking output between dumps
  int i_track_;         ///< Current index of tracking output in the buffer
  int i_dump_;          ///< Current index of the current dump
  std::string prefix_;  ///< Prefix for the output file
  ulong* idtrack_;      ///< Tracked particle ID
  double *xtrack_, *ytrack_, *ztrack_;     ///< Tracked particle location
  double *utrack_, *vtrack_, *wtrack_;     ///< Tracked particle velocity
  double *extrack_, *eytrack_, *eztrack_;  ///< Tracked particle electric field
  double *bxtrack_, *bytrack_, *bztrack_;  ///< Tracked particle magnetic field
};
}  // namespace lili::particle
