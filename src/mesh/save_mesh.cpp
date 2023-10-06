/**
 * @file save_mesh.cpp
 * @brief Source file for saving mesh data
 */
#include "save_mesh.hpp"

#include <fstream>

namespace lili::mesh {
/**
 * @brief Save mesh data to HDF5 file. Not efficient for multiple calls.
 *
 * @param mesh Mesh data
 * @param file_name HDF5 file name
 * @param data_name HDF5 dataset name
 */
void SaveMesh(Mesh<double> &mesh, const char *file_name,
              const char *data_name) {
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
  hsize_t dims[3] = {mesh.nx(), mesh.ny(), mesh.nz()};
  hid_t dataspace_id = H5Screate_simple(3, dims, NULL);

  // Create dataset
  if (file_exists && H5Lexists(file_id, data_name, H5P_DEFAULT) > 0) {
    H5Ldelete(file_id, data_name, H5P_DEFAULT);
  }
  hid_t dataset_id =
      H5Dcreate(file_id, data_name, H5T_NATIVE_DOUBLE, dataspace_id,
                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

  // Write data
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
           mesh.data());

  // Close dataset and dataspace
  H5Dclose(dataset_id);
  H5Dclose(dataset_id);
  H5Sclose(dataspace_id);

  // Close file
  H5Fclose(file_id);
}
}  // namespace lili::mesh