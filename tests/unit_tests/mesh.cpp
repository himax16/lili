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