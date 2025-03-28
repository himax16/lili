/**
 * @file mesh.cpp
 * @brief Source file for additional routines of the Mesh class
 */

#include "mesh.hpp"

#include <fstream>
#include <iomanip>

#include "hdf5.h"
#include "output.hpp"

namespace lili::mesh {
void PrintMeshSize(const MeshSize& mesh_size, lili::output::LiliCout& lout) {
  lout << "=========== Mesh information ===========" << std::endl;
  lout << "dim           = " << mesh_size.dim << std::endl;
  lout << "n             = (" << mesh_size.nx << ", " << mesh_size.ny << ", "
       << mesh_size.nz << ")" << std::endl;
  lout << "ng            = (" << mesh_size.ngx << ", " << mesh_size.ngy << ", "
       << mesh_size.ngz << ")" << std::endl;
  lout << "l             = (" << mesh_size.lx << ", " << mesh_size.ly << ", "
       << mesh_size.lz << ")" << std::endl;
  lout << "r0            = (" << mesh_size.x0 << ", " << mesh_size.y0 << ", "
       << mesh_size.z0 << ")" << std::endl;
}

void UpdateMeshSizeDim(MeshSize& mesh_size) {
  mesh_size.dim = (mesh_size.nz > 1) ? 3 : ((mesh_size.ny > 1) ? 2 : 1);
}

void SaveMesh(Mesh<double>& mesh, const char* file_name, const char* data_name,
              bool include_ghost) {
  // Check if file exists
  hid_t file_id;
  std::ifstream fs(file_name);
  bool file_exists = fs.good();
  if (file_exists) {
    file_exists = H5Fis_hdf5(file_name) > 0;
  }

  if (file_exists) {
    file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
  } else {
    file_id = H5Fcreate(file_name, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  }

  // Create dataspace with mesh size as the final size
  int nx, ny, nz;
  int ixoff, iyoff, izoff;
  if (include_ghost) {
    nx = mesh.nx() + 2 * mesh.ngx();
    ny = mesh.ny() + 2 * mesh.ngy();
    nz = mesh.nz() + 2 * mesh.ngz();

    ixoff = -mesh.ngx();
    iyoff = -mesh.ngy();
    izoff = -mesh.ngz();
  } else {
    nx = mesh.nx();
    ny = mesh.ny();
    nz = mesh.nz();

    ixoff = 0;
    iyoff = 0;
    izoff = 0;
  }

  hsize_t dims[3] = {static_cast<hsize_t>(nx), static_cast<hsize_t>(ny),
                     static_cast<hsize_t>(nz)};
  hid_t dataspace_id = H5Screate_simple(3, dims, NULL);

  // Create dataset
  if (file_exists && H5Lexists(file_id, data_name, H5P_DEFAULT) > 0) {
    H5Ldelete(file_id, data_name, H5P_DEFAULT);
  }
  hid_t dataset_id =
      H5Dcreate(file_id, data_name, H5T_NATIVE_DOUBLE, dataspace_id,
                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  // Prepare data to be written transpose it to row-major order
  double* data;

  data = new double[nx * ny * nz]();
  for (int i = 0; i < nx; ++i) {
    for (int j = 0; j < ny; ++j) {
      for (int k = 0; k < nz; ++k) {
        data[i * ny * nz + j * nz + k] = mesh(i + ixoff, j + iyoff, k + izoff);
      }
    }
  }

  // Write data
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

  // Close dataset and dataspace
  H5Dclose(dataset_id);
  H5Sclose(dataspace_id);

  // Close file
  H5Fclose(file_id);

  // Free memory
  delete[] data;
}

void LoadMeshTo(Mesh<double>& mesh, const char* file_name,
                const char* data_name, bool include_ghost) {
  // Check if file exists
  hid_t file_id;
  std::ifstream fs(file_name);
  bool file_exists = fs.good();
  if (file_exists) {
    file_exists = H5Fis_hdf5(file_name) > 0;
  }

  if (file_exists) {
    file_id = H5Fopen(file_name, H5F_ACC_RDWR, H5P_DEFAULT);
  } else {
    std::cerr << "File " << file_name << " does not exist..." << std::endl;
    exit(2);
  }

  // Check if dataset exists
  if (H5Lexists(file_id, data_name, H5P_DEFAULT) <= 0) {
    std::cerr << "Dataset " << data_name << " does not exist..." << std::endl;
    exit(2);
  }

  // Open dataset
  hid_t dataset_id = H5Dopen(file_id, data_name, H5P_DEFAULT);

  // Get dataset size
  hid_t dataspace_id = H5Dget_space(dataset_id);
  const int ndims = H5Sget_simple_extent_ndims(dataspace_id);
  hsize_t dims[ndims];
  H5Sget_simple_extent_dims(dataspace_id, dims, NULL);

  // Resize mesh if needed
  int ndx = dims[0];
  int ndy = ndims > 1 ? dims[1] : 1;
  int ndz = ndims > 2 ? dims[2] : 1;
  int ngx = mesh.ngx();
  int ngy = mesh.ngy();
  int ngz = mesh.ngz();

  if (include_ghost) {
    mesh.Resize(ndx - 2 * ngx, ndy - 2 * ngy, ndz - 2 * ngz, ngx, ngy, ngz);
  } else {
    mesh.Resize(ndx, ndy, ndz, ngx, ngy, ngz);
  }

  // Read data
  double* data = new double[ndx * ndy * ndz]();
  H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

  // Copy data to mesh
  int ixoff, iyoff, izoff;
  if (include_ghost) {
    ixoff = -ngx;
    iyoff = -ngy;
    izoff = -ngz;
  } else {
    ixoff = 0;
    iyoff = 0;
    izoff = 0;
  }

  for (int i = 0; i < ndx; ++i) {
    for (int j = 0; j < ndy; ++j) {
      for (int k = 0; k < ndz; ++k) {
        mesh(i + ixoff, j + iyoff, k + izoff) =
            data[i * ndy * ndz + j * ndz + k];
      }
    }
  }

  // Close dataset and dataspace
  H5Dclose(dataset_id);
  H5Sclose(dataspace_id);

  // Close file
  H5Fclose(file_id);

  // Clean up
  delete[] data;
}
}  // namespace lili::mesh
