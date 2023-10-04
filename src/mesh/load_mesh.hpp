/**
 * @file load_mesh.hpp
 * @brief Header file for loading mesh from input file
 */
#pragma once

#include <iostream>

#include "hdf5.h"
#include "mesh.hpp"

namespace lili::mesh {
// Function declaration
void LoadMesh(char *in_file, char *dataset_name, Mesh<double> &mesh);

Mesh<double> LoadMesh(char *in_file, char *dataset_name);
}  // namespace lili::mesh
