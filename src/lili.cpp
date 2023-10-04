/**
 * @file lili.cpp
 * @brief Main LILI program
 *
 */

// #include <numbers>

#include <iomanip>

#include "hdf5.h"
#include "input.hpp"
#include "mesh.hpp"
#include "save_mesh.hpp"

/**
 * @brief Main `LILI` program
 *
 * @param[in] argc
 *  Number of command line arguments
 * @param[in] argv
 *  Command line arguments
 */
int main(int argc, char *argv[]) {
  // Parse inputs
  lili::input::ParseArguments(argc, argv);

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

  /****************************************************************************/
  // Variable declaration
  // int n_simsys = 0;
  // bool has_mhd = false;
  // bool use_emf = true;

  // int i_loop = 0;
  // int n_loop = 10;

  // // Initialize simulation system based on the input file
  // if (n_simsys <= 0) {
  //   std::cerr << "Number of simulated system must be positive" << std::endl;
  //   exit(10);
  // }
  // for (int i_simsys = 0; i_simsys < n_simsys; ++i_simsys) {
  //   switch (simsys[i_simsys].type) {
  //     case 'm':
  //       if (has_mhd) {
  //         std::cerr << "There are multiple MHD simulation system" <<
  //         std::endl; exit(12);
  //       }
  //       initialize_mhdsys(simsys[i_simsys].property);
  //       has_mhd = true;
  //     case 'p':
  //       initialize_parsys(simsys[i_simsys].property);
  //       break;
  //     case 'f':
  //       initialize_flusys(simsys[i_simsys].property);
  //       break;
  //     case 'n':
  //       initialize_neusys(simsys[i_simsys].property);
  //       break;
  //     default:
  //       std::cerr << "Unrecognized simulation system" << std::endl;
  //       exit(11);
  //   }
  // }

  // // Initialize field variable if needed
  // if (use_emf) {
  //   initialize_emfsys();
  // }

  // // boost::json::object obj;
  // // obj["pi"] = boost::math::constants::pi<double>();

  // // std::cout << obj << std::endl;

  // // LILI main loop
  // for (i_loop = 0; i_loop < n_loop; ++i_loop) {
  //   std::cout << i_loop << std::endl;

  // }

  return 0;
}
