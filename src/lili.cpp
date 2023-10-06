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
#include "particle.hpp"
#include "particle_initialization.hpp"

using namespace lili;
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
  input::Input input = input::ParseArguments(argc, argv);

  // Print input
  std::cout << "Input file   : " << input.input_file() << std::endl;
  std::cout << "Problem name : " << input.problem_name() << std::endl;
  std::cout << "Input type   : " << input.input_type() << std::endl;

  // Print input mesh information
  std::cout << "====== Mesh information ======" << std::endl;
  std::cout << "dim = " << input.mesh().dim << std::endl;
  std::cout << "lx  = " << input.mesh().lx << std::endl;
  std::cout << "nx  = " << input.mesh().nx << std::endl;
  std::cout << "ngx = " << input.mesh().ngx << std::endl;
  std::cout << std::endl;

  std::cout << "ly  = " << input.mesh().ly << std::endl;
  std::cout << "ny  = " << input.mesh().ny << std::endl;
  std::cout << "ngy = " << input.mesh().ngy << std::endl;
  std::cout << std::endl;

  std::cout << "lz  = " << input.mesh().lz << std::endl;
  std::cout << "nz  = " << input.mesh().nz << std::endl;
  std::cout << "ngz = " << input.mesh().ngz << std::endl;
  std::cout << "==============================" << std::endl;

  // Print input particle information
  std::cout << "==== Particle information ====" << std::endl;
  for (input::InputParticle particle : input.particles()) {
    std::cout << "== " << particle.name << std::endl;
    std::cout << "  n   = " << particle.n << std::endl;
    std::cout << "  m   = " << particle.m << std::endl;
    std::cout << "  q   = " << particle.q << std::endl;
    std::cout << "  tau = " << particle.tau << std::endl;
  }
  std::cout << "==============================" << std::endl;

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
