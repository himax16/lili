/**
 * @file track_particle.cpp
 * @brief Source file for the TrackParticles helper class for tracking particles
 */

#include "track_particle.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "fields.hpp"
#include "hdf5.h"
#include "mesh.hpp"

namespace lili::particle {
void TrackParticles::InitializeTrackParticles() {
  // Initialize the particles
  track_particles = Particles(n_track_);

  // Allocate memory for the tracked particles
  idtrack_ = new ulong[n_track_ * dtrack_save_]();
  xtrack_ = new double[n_track_ * dtrack_save_]();
  ytrack_ = new double[n_track_ * dtrack_save_]();
  ztrack_ = new double[n_track_ * dtrack_save_]();
  utrack_ = new double[n_track_ * dtrack_save_]();
  vtrack_ = new double[n_track_ * dtrack_save_]();
  wtrack_ = new double[n_track_ * dtrack_save_]();
  extrack_ = new double[n_track_ * dtrack_save_]();
  eytrack_ = new double[n_track_ * dtrack_save_]();
  eztrack_ = new double[n_track_ * dtrack_save_]();
  bxtrack_ = new double[n_track_ * dtrack_save_]();
  bytrack_ = new double[n_track_ * dtrack_save_]();
  bztrack_ = new double[n_track_ * dtrack_save_]();
}

void TrackParticles::SaveTrackedParticles(Particles& particles) {
  // Copy tracked particles to the current cache
  SelectParticles(particles, track_particles, ParticleStatus::Tracked);
  if (track_particles.npar() != n_track_) {
    std::cerr << "Error: number of tracked particles is not correct"
              << std::endl;
    exit(1);
  }

  // Move the data to the dump cache
  for (int i_track = 0; i_track < n_track_; ++i_track) {
    idtrack_[i_track_ * n_track_ + i_track] = track_particles.id(i_track);
    xtrack_[i_track_ * n_track_ + i_track] = track_particles.x(i_track);
    ytrack_[i_track_ * n_track_ + i_track] = track_particles.y(i_track);
    ztrack_[i_track_ * n_track_ + i_track] = track_particles.z(i_track);
    utrack_[i_track_ * n_track_ + i_track] = track_particles.u(i_track);
    vtrack_[i_track_ * n_track_ + i_track] = track_particles.v(i_track);
    wtrack_[i_track_ * n_track_ + i_track] = track_particles.w(i_track);
  }

  // Increment the tracking index
  ++i_track_;

  // Save the tracked particles if the tracking index reaches the save number
  if (i_track_ >= dtrack_save_) {
    DumpTrackedParticles();
  }
}

void TrackParticles::SaveTrackedParticles(Particles& particles,
                                          mesh::Fields& fields) {
  // Copy tracked particles to the current cache
  SelectParticles(particles, track_particles, ParticleStatus::Tracked);
  if (track_particles.npar() != n_track_) {
    std::cout << "Error: number of tracked particles is not correct"
              << std::endl;
    exit(1);
  }

  // Move the data to the dump cache
  for (int i_track = 0; i_track < n_track_; ++i_track) {
    idtrack_[i_track_ * n_track_ + i_track] = track_particles.id(i_track);

    double xloc = track_particles.x(i_track);
    double yloc = track_particles.y(i_track);
    double zloc = track_particles.z(i_track);

    xtrack_[i_track_ * n_track_ + i_track] = xloc;
    ytrack_[i_track_ * n_track_ + i_track] = yloc;
    ztrack_[i_track_ * n_track_ + i_track] = zloc;
    utrack_[i_track_ * n_track_ + i_track] = track_particles.u(i_track);
    vtrack_[i_track_ * n_track_ + i_track] = track_particles.v(i_track);
    wtrack_[i_track_ * n_track_ + i_track] = track_particles.w(i_track);

    // Move the particle location to the mesh coordinate
    xloc = (xloc - fields.size.x0) / fields.size.lx * fields.size.nx;
    yloc = (yloc - fields.size.y0) / fields.size.ly * fields.size.ny;
    zloc = (zloc - fields.size.z0) / fields.size.lz * fields.size.nz;

    // Store the fields
    extrack_[i_track_ * n_track_ + i_track] =
        fields.ex.Interpolation(xloc, yloc, zloc);
    eytrack_[i_track_ * n_track_ + i_track] =
        fields.ey.Interpolation(xloc, yloc, zloc);
    eztrack_[i_track_ * n_track_ + i_track] =
        fields.ez.Interpolation(xloc, yloc, zloc);
    bxtrack_[i_track_ * n_track_ + i_track] =
        fields.bx.Interpolation(xloc, yloc, zloc);
    bytrack_[i_track_ * n_track_ + i_track] =
        fields.by.Interpolation(xloc, yloc, zloc);
    bztrack_[i_track_ * n_track_ + i_track] =
        fields.bz.Interpolation(xloc, yloc, zloc);
  }

  // Increment the tracking index
  ++i_track_;

  // Save the tracked particles if the tracking index reaches the save number
  if (i_track_ >= dtrack_save_) {
    DumpTrackedParticles();
  }
}

void TrackParticles::DumpTrackedParticles() {
  // Set filename and create file
  std::stringstream ss;
  ss << std::setw(5) << std::setfill('0') << i_dump_;
  std::string filename = prefix_ + "_" + ss.str() + ".h5";
  std::cout << "Dumping tracked particles to " << filename << std::endl;
  hid_t file_id =
      H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Create dataspace to store particles
  if (i_track_ != dtrack_save_) {
    std::cout << "Different number TP dump: " << i_track_ << " " << dtrack_save_
              << std::endl;
  }
  hsize_t dims[2] = {static_cast<hsize_t>(i_track_),
                     static_cast<hsize_t>(n_track_)};
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

  // Write the particle local fields
  dataset_id = H5Dcreate(file_id, "ex", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           extrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "ey", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           eytrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "ez", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           eztrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "bx", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           bxtrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "by", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           bytrack_);
  H5Dclose(dataset_id);
  dataset_id = H5Dcreate(file_id, "bz", H5T_NATIVE_DOUBLE, dataspace_id,
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           bztrack_);
  H5Dclose(dataset_id);

  // Close dataspace and file
  H5Sclose(dataspace_id);
  H5Fclose(file_id);

  // Add the dump index and clear the tracking index
  ++i_dump_;
  i_track_ = 0;
}
}  // namespace lili::particle
