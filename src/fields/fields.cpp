/**
 * @file fields.cpp
 * @brief Source file for additional routines of the Fields class
 */
#include "fields.hpp"
#include "mesh.hpp"

namespace lili::mesh {
/**
 * @brief Function to load Fields data from a file
 * @param fields Fields data
 * @param file_name HDF5 file name
 * @param include_ghost Include ghost cells in the loaded data
 *
 * @todo Check sizes after loading and make this a single read file
 */
void LoadFieldTo(Fields& fields, const char* file_name, bool include_ghost) {
  mesh::LoadMeshTo(fields.ex, file_name, "ex", include_ghost);
  mesh::LoadMeshTo(fields.ey, file_name, "ey", include_ghost);
  mesh::LoadMeshTo(fields.ez, file_name, "ez", include_ghost);
  mesh::LoadMeshTo(fields.bx, file_name, "bx", include_ghost);
  mesh::LoadMeshTo(fields.by, file_name, "by", include_ghost);
  mesh::LoadMeshTo(fields.bz, file_name, "bz", include_ghost);
}
}  // namespace lili::mesh
