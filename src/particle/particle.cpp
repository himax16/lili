/**
 * @file particle.cpp
 * @brief Source file for the Particles class
 */

#include "particle.hpp"

#include <fstream>

#include "hdf5.h"

namespace lili::particle {
const char* __LILIP_DNAME_UINT32[] = {"id", "status"};
const char* __LILIP_DNAME_DOUBLE[] = {"x", "y", "z", "u", "v", "w"};

// Constructor
Particles::Particles()
    : npar_(0),
      npar_max_(__LILIP_DEFAULT_BSIZE),
      q_(1.0),
      m_(1.0),
      id_(new ulong[__LILIP_DEFAULT_BSIZE]()),
      status_(new ParticleStatus[__LILIP_DEFAULT_BSIZE]()),
      x_(new double[__LILIP_DEFAULT_BSIZE]()),
      y_(new double[__LILIP_DEFAULT_BSIZE]()),
      z_(new double[__LILIP_DEFAULT_BSIZE]()),
      u_(new double[__LILIP_DEFAULT_BSIZE]()),
      v_(new double[__LILIP_DEFAULT_BSIZE]()),
      w_(new double[__LILIP_DEFAULT_BSIZE]()) {}

Particles::Particles(int npar)
    : npar_(npar),
      npar_max_(npar < __LILIP_DEFAULT_BSIZE ? __LILIP_DEFAULT_BSIZE : npar),
      q_(1.0),
      m_(1.0),
      id_(nullptr),
      status_(nullptr),
      x_(nullptr),
      y_(nullptr),
      z_(nullptr),
      u_(nullptr),
      v_(nullptr),
      w_(nullptr) {
  id_ = new ulong[npar_max_]();
  status_ = new ParticleStatus[npar_max_]();
  x_ = new double[npar_max_]();
  y_ = new double[npar_max_]();
  z_ = new double[npar_max_]();
  u_ = new double[npar_max_]();
  v_ = new double[npar_max_]();
  w_ = new double[npar_max_]();

  // Initialize status
  for (int i = 0; i < npar_; ++i) {
    status_[i] = ParticleStatus::In;
  }
}

Particles::Particles(int npar, int npar_max)
    : npar_(npar),
      npar_max_(npar_max),
      q_(1.0),
      m_(1.0),
      id_(new ulong[npar_max]()),
      status_(new ParticleStatus[npar_max]()),
      x_(new double[npar_max]()),
      y_(new double[npar_max]()),
      z_(new double[npar_max]()),
      u_(new double[npar_max]()),
      v_(new double[npar_max]()),
      w_(new double[npar_max]()) {
  // Initialize status
  for (int i = 0; i < npar_; ++i) {
    status_[i] = ParticleStatus::In;
  }
}

Particles::Particles(input::InputParticles input_particle)
    : npar_(input_particle.n),
      npar_max_(input_particle.n < __LILIP_DEFAULT_BSIZE ? __LILIP_DEFAULT_BSIZE
                                                         : input_particle.n),
      q_(input_particle.q),
      m_(input_particle.m),
      id_(nullptr),
      status_(nullptr),
      x_(nullptr),
      y_(nullptr),
      z_(nullptr),
      u_(nullptr),
      v_(nullptr),
      w_(nullptr) {
  id_ = new ulong[npar_max_]();
  status_ = new ParticleStatus[npar_max_]();
  x_ = new double[npar_max_]();
  y_ = new double[npar_max_]();
  z_ = new double[npar_max_]();
  u_ = new double[npar_max_]();
  v_ = new double[npar_max_]();
  w_ = new double[npar_max_]();

  // Initialize status
  for (int i = 0; i < npar_; ++i) {
    status_[i] = ParticleStatus::In;
  }
}

// Copy constructor
Particles::Particles(const Particles& other)
    : npar_(other.npar_),
      npar_max_(other.npar_max_),
      q_(other.q_),
      m_(other.m_),
      id_(new ulong[other.npar_max_]()),
      status_(new ParticleStatus[other.npar_max_]()),
      x_(new double[other.npar_max_]()),
      y_(new double[other.npar_max_]()),
      z_(new double[other.npar_max_]()),
      u_(new double[other.npar_max_]()),
      v_(new double[other.npar_max_]()),
      w_(new double[other.npar_max_]()) {
  std::copy(other.id_, other.id_ + other.npar_max_, id_);
  std::copy(other.status_, other.status_ + other.npar_max_, status_);
  std::copy(other.x_, other.x_ + other.npar_max_, x_);
  std::copy(other.y_, other.y_ + other.npar_max_, y_);
  std::copy(other.z_, other.z_ + other.npar_max_, z_);
  std::copy(other.u_, other.u_ + other.npar_max_, u_);
  std::copy(other.v_, other.v_ + other.npar_max_, v_);
  std::copy(other.w_, other.w_ + other.npar_max_, w_);
}

// Destructor
Particles::~Particles() {
  delete[] id_;
  delete[] status_;
  delete[] x_;
  delete[] y_;
  delete[] z_;
  delete[] u_;
  delete[] v_;
  delete[] w_;
}

void Particles::resize(int new_npar_max) {
  ulong* new_id = new ulong[new_npar_max]();
  ParticleStatus* new_status = new ParticleStatus[new_npar_max]();
  double* new_x = new double[new_npar_max]();
  double* new_y = new double[new_npar_max]();
  double* new_z = new double[new_npar_max]();
  double* new_u = new double[new_npar_max]();
  double* new_v = new double[new_npar_max]();
  double* new_w = new double[new_npar_max]();

  std::copy(id_, id_ + npar_max_, new_id);
  std::copy(status_, status_ + npar_max_, new_status);
  std::copy(x_, x_ + npar_max_, new_x);
  std::copy(y_, y_ + npar_max_, new_y);
  std::copy(z_, z_ + npar_max_, new_z);
  std::copy(u_, u_ + npar_max_, new_u);
  std::copy(v_, v_ + npar_max_, new_v);
  std::copy(w_, w_ + npar_max_, new_w);

  delete[] id_;
  delete[] status_;
  delete[] x_;
  delete[] y_;
  delete[] z_;
  delete[] u_;
  delete[] v_;
  delete[] w_;

  id_ = new_id;
  status_ = new_status;
  x_ = new_x;
  y_ = new_y;
  z_ = new_z;
  u_ = new_u;
  v_ = new_v;
  w_ = new_w;

  npar_max_ = new_npar_max;
}

void Particles::AddID(int offset) {
  for (int i = 0; i < npar_; ++i) {
    id_[i] += offset;
  }
}

void Particles::pswap(const int i, const int j) {
  int tmp_uint32 = id_[i];
  id_[i] = id_[j];
  id_[j] = tmp_uint32;

  ParticleStatus tmp_status = status_[i];
  status_[i] = status_[j];
  status_[j] = tmp_status;

  double tmp_double = x_[i];
  x_[i] = x_[j];
  x_[j] = tmp_double;

  tmp_double = y_[i];
  y_[i] = y_[j];
  y_[j] = tmp_double;

  tmp_double = z_[i];
  z_[i] = z_[j];
  z_[j] = tmp_double;

  tmp_double = u_[i];
  u_[i] = u_[j];
  u_[j] = tmp_double;

  tmp_double = v_[i];
  v_[i] = v_[j];
  v_[j] = tmp_double;

  tmp_double = w_[i];
  w_[i] = w_[j];
  w_[j] = tmp_double;
}

void Particles::CleanOut() {
  // Keep two indices
  int i = 0;
  int j = npar_ - 1;

  // Loop until i and j meet, swap particles if necessary
  while (i < j) {
    if (status_[i] == ParticleStatus::Out) {
      while (status_[j] == ParticleStatus::Out) {
        --j;
        if (i >= j) {
          break;
        }
      }
      if (i >= j) {
        break;
      }
      pswap(i, j);
      --j;
    }
    ++i;
  }

  // Check the last particle
  if (status_[i] == ParticleStatus::Out) {
    --i;
  }

  // Change the number of particles
  npar_ = i + 1;
}

/**
 * @brief Function to save particle data
 * @param particles Particles object
 * @param file_name Name of the file to save to
 */
void SaveParticles(Particles& particles, const char* file_name) {
  // Create file
  hid_t file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  // Create dataspace to store particles
  hsize_t dims[1] = {static_cast<hsize_t>(particles.npar())};
  hid_t dataspace_id = H5Screate_simple(1, dims, NULL);

  // Create dataset for each integer fields
  for (int i = 0; i < __LILIP_DCOUNT_ULONG; ++i) {
    hid_t dataset_id =
        H5Dcreate(file_id, __LILIP_DNAME_UINT32[i], H5T_NATIVE_UINT32,
                  dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Write data
    H5Dwrite(dataset_id, H5T_NATIVE_UINT32, H5S_ALL, H5S_ALL, H5P_DEFAULT,
             particles.data_uint32(i));

    // Close dataset
    H5Dclose(dataset_id);
  }

  // Create dataset for each double fields
  for (int i = 0; i < __LILIP_DCOUNT_DOUBLE; ++i) {
    hid_t dataset_id =
        H5Dcreate(file_id, __LILIP_DNAME_DOUBLE[i], H5T_NATIVE_DOUBLE,
                  dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Write data
    H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
             particles.data_double(i));

    // Close dataset
    H5Dclose(dataset_id);
  }

  // Close dataspace
  H5Sclose(dataspace_id);

  // Close file
  H5Fclose(file_id);
}

Particles LoadParticles(const char* file_name) {
  // Open file
  hid_t file_id = H5Fopen(file_name, H5F_ACC_RDONLY, H5P_DEFAULT);

  // Get number of particles
  hsize_t dims[1];
  hid_t dataset_id = H5Dopen(file_id, __LILIP_DNAME_UINT32[0], H5P_DEFAULT);
  hid_t dataspace_id = H5Dget_space(dataset_id);
  H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
  int npar = dims[0];

  // Create particles object
  Particles particles(npar);

  // Read data
  for (int i = 0; i < __LILIP_DCOUNT_ULONG; ++i) {
    dataset_id = H5Dopen(file_id, __LILIP_DNAME_UINT32[i], H5P_DEFAULT);
    H5Dread(dataset_id, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            particles.data_uint32(i));
    H5Dclose(dataset_id);
  }
  for (int i = 0; i < __LILIP_DCOUNT_DOUBLE; ++i) {
    dataset_id = H5Dopen(file_id, __LILIP_DNAME_DOUBLE[i], H5P_DEFAULT);
    H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
            particles.data_double(i));
    H5Dclose(dataset_id);
  }

  // Close dataspace
  H5Sclose(dataspace_id);

  // Close file
  H5Fclose(file_id);

  return particles;
}

void SelectParticles(Particles& input, Particles& output, ParticleStatus status,
                     bool remove) {
  int npar = input.npar();

  // Loop over input particles
  int npar_out = 0;
  for (int i = 0; i < npar; ++i) {
    if (input.status(i) == status) {
      // Grow the output particles if necessary
      if (npar_out >= output.npar_max()) {
        output.resize(output.npar_max() * __LILIP_DEFAULT_GSIZE);
      }

      output.id(npar_out) = input.id(i);
      output.status(npar_out) = input.status(i);
      output.x(npar_out) = input.x(i);
      output.y(npar_out) = input.y(i);
      output.z(npar_out) = input.z(i);
      output.u(npar_out) = input.u(i);
      output.v(npar_out) = input.v(i);
      output.w(npar_out) = input.w(i);
      ++npar_out;
      if (remove) {
        input.status(i) = ParticleStatus::Out;
      }
    }
  }

  // Change the number of particles in output
  output.npar() = npar_out;

  // Clean up input particles
  if (remove) {
    input.CleanOut();
  }
}

void LabelBoundaryParticles(Particles& particles, mesh::MeshSize mesh_size) {
  // Get the range of each dimension
  const double xmin = mesh_size.x0;
  const double xmax = mesh_size.x0 + mesh_size.lx;
  const double ymin = mesh_size.y0;
  const double ymax = mesh_size.y0 + mesh_size.ly;
  const double zmin = mesh_size.z0;
  const double zmax = mesh_size.z0 + mesh_size.lz;

  double* __restrict__ x = particles.x();
  double* __restrict__ y = particles.y();
  double* __restrict__ z = particles.z();
  ParticleStatus* __restrict__ status = particles.status();

  // Loop over particles and label them
  // TODO: Can probably improve this branching logic
  for (int i = 0; i < particles.npar(); ++i) {
    if (status[i] == ParticleStatus::Tracked) {
      if (x[i] < xmin) {
        if (y[i] < ymin) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TX0Y0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TX0Y0Z1;
          } else {
            status[i] = ParticleStatus::TX0Y0;
          }
        } else if (y[i] > ymax) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TX0Y1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TX0Y1Z1;
          } else {
            status[i] = ParticleStatus::TX0Y1;
          }
        } else {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TX0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TX0Z1;
          } else {
            status[i] = ParticleStatus::TX0;
          }
        }
      } else if (x[i] > xmax) {
        if (y[i] < ymin) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TX1Y0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TX1Y0Z1;
          } else {
            status[i] = ParticleStatus::TX1Y0;
          }
        } else if (y[i] > ymax) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TX1Y1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TX1Y1Z1;
          } else {
            status[i] = ParticleStatus::TX1Y1;
          }
        } else {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TX1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TX1Z1;
          } else {
            status[i] = ParticleStatus::TX1;
          }
        }
      } else {
        if (y[i] < ymin) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TY0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TY0Z1;
          } else {
            status[i] = ParticleStatus::TY0;
          }
        } else if (y[i] > ymax) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TY1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TY1Z1;
          } else {
            status[i] = ParticleStatus::TY1;
          }
        } else {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::TZ0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::TZ1;
          }
        }
      }
    } else {
      if (x[i] < xmin) {
        if (y[i] < ymin) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::X0Y0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::X0Y0Z1;
          } else {
            status[i] = ParticleStatus::X0Y0;
          }
        } else if (y[i] > ymax) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::X0Y1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::X0Y1Z1;
          } else {
            status[i] = ParticleStatus::X0Y1;
          }
        } else {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::X0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::X0Z1;
          } else {
            status[i] = ParticleStatus::X0;
          }
        }
      } else if (x[i] > xmax) {
        if (y[i] < ymin) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::X1Y0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::X1Y0Z1;
          } else {
            status[i] = ParticleStatus::X1Y0;
          }
        } else if (y[i] > ymax) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::X1Y1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::X1Y1Z1;
          } else {
            status[i] = ParticleStatus::X1Y1;
          }
        } else {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::X1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::X1Z1;
          } else {
            status[i] = ParticleStatus::X1;
          }
        }
      } else {
        if (y[i] < ymin) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::Y0Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::Y0Z1;
          } else {
            status[i] = ParticleStatus::Y0;
          }
        } else if (y[i] > ymax) {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::Y1Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::Y1Z1;
          } else {
            status[i] = ParticleStatus::Y1;
          }
        } else {
          if (z[i] < zmin) {
            status[i] = ParticleStatus::Z0;
          } else if (z[i] > zmax) {
            status[i] = ParticleStatus::Z1;
          }
        }
      }
    }
  }
}

void PeriodicBoundaryParticles(Particles& particles, mesh::MeshSize mesh_size) {
  // Get the range of each dimension
  const double lx = mesh_size.lx;
  const double ly = mesh_size.ly;
  const double lz = mesh_size.lz;

  const double xmin = mesh_size.x0;
  const double xmax = mesh_size.x0 + lx;
  const double ymin = mesh_size.y0;
  const double ymax = mesh_size.y0 + ly;
  const double zmin = mesh_size.z0;
  const double zmax = mesh_size.z0 + lz;

  double* __restrict__ x = particles.x();
  double* __restrict__ y = particles.y();
  double* __restrict__ z = particles.z();

  // Loop over particles and move them
  for (int i = 0; i < particles.npar(); ++i) {
    if (x[i] < xmin) {
      x[i] += lx;
    } else if (x[i] > xmax) {
      x[i] -= lx;
    }
    if (y[i] < ymin) {
      y[i] += ly;
    } else if (y[i] > ymax) {
      y[i] -= ly;
    }
    if (z[i] < zmin) {
      z[i] += lz;
    } else if (z[i] > zmax) {
      z[i] -= lz;
    }
  }
}
}  // namespace lili::particle
