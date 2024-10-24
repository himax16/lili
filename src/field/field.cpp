/**
 * @file field.cpp
 * @brief Source file for additional routines of the Field class
 */
#include "field.hpp"

#include "mesh.hpp"

namespace lili::mesh {
/**
 * @brief Function to load Field data from a file
 * @param field Field data
 * @param file_name HDF5 file name
 * @param include_ghost Include ghost cells in the loaded data
 *
 * @todo Check sizes after loading and make this a single read file
 */
void LoadFieldTo(Field& field, const char* file_name, bool include_ghost) {
  mesh::LoadMeshTo(field.ex, file_name, "ex", include_ghost);
  mesh::LoadMeshTo(field.ey, file_name, "ey", include_ghost);
  mesh::LoadMeshTo(field.ez, file_name, "ez", include_ghost);
  mesh::LoadMeshTo(field.bx, file_name, "bx", include_ghost);
  mesh::LoadMeshTo(field.by, file_name, "by", include_ghost);
  mesh::LoadMeshTo(field.bz, file_name, "bz", include_ghost);
}
}  // namespace lili::mesh
