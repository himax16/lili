// Test copy
lili::mesh::Mesh<double> mesh2(mesh);
std::cout << "Data loc: " << mesh2.data() << std::endl;

// Print data
for (int k = 0; k < n2; ++k) {
  for (int j = 0; j < n1; ++j) {
    for (int i = 0; i < n0; ++i) {
      std::cout << std::setw(3) << mesh2(i, j, k) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
std::cout << "----" << std::endl;

// Test assignment operator
lili::mesh::Mesh<double> mesh3 = mesh;
std::cout << "Data loc: " << mesh3.data() << std::endl;

// Print data
for (int k = 0; k < n2; ++k) {
  for (int j = 0; j < n1; ++j) {
    for (int i = 0; i < n0; ++i) {
      std::cout << std::setw(3) << mesh3(i, j, k) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

// Test mesh
u_int32_t n0 = 5;
u_int32_t n1 = 3;
u_int32_t n2 = 4;
lili::mesh::MeshSize msize = {n0, n1, n2};

lili::mesh::Mesh<double> mesh(msize);

std::cout << "Data loc: " << mesh.data() << std::endl;

// Assign data
for (int k = 0; k < n2; ++k) {
  for (int j = 0; j < n1; ++j) {
    for (int i = 0; i < n0; ++i) {
      mesh(i, j, k) = i + n0 * j + n0 * n1 * k;
    }
  }
}

// Print data
for (int k = 0; k < n2; ++k) {
  for (int j = 0; j < n1; ++j) {
    for (int i = 0; i < n0; ++i) {
      std::cout << std::setw(3) << mesh(i, j, k) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
std::cout << "----" << std::endl;

// Save mesh
// lili::mesh::SaveMesh(mesh, "test.h5", "test");
// lili::mesh::SaveMesh(mesh, "test.h5", "test2");
// lili::mesh::SaveMeshHyperslab(mesh, {1, 2, 3}, {2, 2, 2}, "test.h5",
// "test");

// Test Mesh
mesh::Mesh<double> bz(input.mesh());

// Set mesh values according to its X location
double lx = input.mesh().lx;
for (int k = 0; k < bz.nz(); ++k) {
  for (int j = 0; j < bz.ny(); ++j) {
    for (int i = 0; i < bz.nx(); ++i) {
      bz(i, j, k) = lx * i / bz.nx();
    }
  }
}

// Test boundary condition
bz.CopyToGhost(bz, mesh::MeshGhostLocation::XPrev);
bz.CopyToGhost(bz, mesh::MeshGhostLocation::XNext);

mesh::SaveMesh(bz, "test.h5", "bz", true);

// Test load
mesh::Mesh<double> bz_load;
mesh::LoadMeshTo(bz_load, "test.h5", "bz", false);
bz_load.Shrink(bz.nx(), bz.ny(), bz.nz(), bz.ngx(), bz.ngy(), bz.ngz());
mesh::SaveMesh(bz_load, "test_copy.h5", "bz", true);
