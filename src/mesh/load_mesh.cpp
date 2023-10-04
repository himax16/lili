/**
 * @file load_mesh.cpp
 * @brief Implementation file for loading mesh from input file
 */
#include "load_mesh.hpp"

namespace lili::mesh {
/**
 * @brief Load mesh from input file
 *
 * @param[in] in_file
 * Input file name
 * @param[in] dataset_name
 * Dataset name in input file
 * @param[out] mesh
 * Mesh data
 */
void LoadMesh(char *in_file, char *dataset_name, Mesh<double> &mesh) {
  // Open file
  hid_t file_id = H5Fopen(in_file, H5F_ACC_RDONLY, H5P_DEFAULT);

  // Open dataset
  hid_t dataset_id = H5Dopen(file_id, dataset_name, H5P_DEFAULT);

  // Get dataspace
  hid_t dataspace_id = H5Dget_space(dataset_id);

  // Get dimensions
  hsize_t dims[3];
  H5Sget_simple_extent_dims(dataspace_id, dims, NULL);

  // Read data
  mesh = Mesh<double>(dims[0], dims[1], dims[2]);
  H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
          mesh.data());

  // Close dataset and dataspace
  H5Dclose(dataset_id);
  H5Sclose(dataspace_id);

  // Close file
  H5Fclose(file_id);
}
}  // namespace lili::mesh