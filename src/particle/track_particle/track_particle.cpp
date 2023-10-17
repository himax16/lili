/**
 * @file track_particle.cpp
 * @brief Source file for the TrackParticle helper class for tracking particles
 */

#include "track_particle.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "hdf5.h"

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
    idtrack_[itrack_ * ntrack_ + i_track] = track_particles.id()[i_track];
    xtrack_[itrack_ * ntrack_ + i_track] = track_particles.x()[i_track];
    ytrack_[itrack_ * ntrack_ + i_track] = track_particles.y()[i_track];
    ztrack_[itrack_ * ntrack_ + i_track] = track_particles.z()[i_track];
    utrack_[itrack_ * ntrack_ + i_track] = track_particles.u()[i_track];
    vtrack_[itrack_ * ntrack_ + i_track] = track_particles.v()[i_track];
    wtrack_[itrack_ * ntrack_ + i_track] = track_particles.w()[i_track];
  }

  // Increment the tracking index
  ++itrack_;

  // Dump the tracked particles if the tracking index reaches the dump number
  if (itrack_ >= ndump_) {
    DumpTrackedParticles();
  }
}

/**
 * @brief Dump tracked particles
 */
void TrackParticle::DumpTrackedParticles() {
  // Dump the tracked particles
  // Set filename and create file
  std::stringstream ss;
  ss << std::setw(5) << std::setfill('0') << idump_;
  std::string filename = prefix_ + "_" + ss.str() + ".h5";
  std::cout << "Dumping tracked particles to " << filename << std::endl;
  hid_t file_id =
      H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Create dataspace to store particles
  if (itrack_ != ndump_) {
    std::cout << "Different number TP dump: " << itrack_ << " " << ndump_
              << std::endl;
  }
  hsize_t dims[2] = {static_cast<hsize_t>(itrack_),
                     static_cast<hsize_t>(ntrack_)};
  hid_t dataspace_id = H5Screate_simple(2, dims, NULL);

  // Write the particle IDs
  hid_t dataset_id = H5Dcreate(file_id, "id", H5T_NATIVE_ULONG, dataspace_id,
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           idtrack_);
  H5Dclose(dataset_id);

  // Write the particle coordinates
  dataset_id = H5Dcreate(file_id, "x", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           xtrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "y", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           ytrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "z", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           ztrack_);
  H5Dclose(dataset_id);

  // Write the particle velocities
  dataset_id = H5Dcreate(file_id, "u", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           utrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "v", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           vtrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "w", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           wtrack_);
  H5Dclose(dataset_id);

  // Close dataspace and file
  H5Sclose(dataspace_id);
  H5Fclose(file_id);

  // Add the dump index and clear the tracking index
  ++idump_;
  itrack_ = 0;
}
}  // namespace lili::particle