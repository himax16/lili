/**
 * @file save_mesh.hpp
 * @brief Header file for saving mesh data
 */
#pragma once

#include <cstdint>

#include "hdf5.h"
#include "mesh.hpp"

namespace lili::mesh {
// Function declaration
void SaveMesh(Mesh<double> &mesh, const char *file_name, const char *data_name);
}  // namespace lili::mesh