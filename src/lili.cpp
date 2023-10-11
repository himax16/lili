/**
 * @file lili.cpp
 * @brief Main LILI program
 *
 */

// #include <numbers>

#include <chrono>
#include <iomanip>
#include <thread>

#include "field.hpp"
#include "hdf5.h"
#include "input.hpp"
#include "mesh.hpp"
#include "particle.hpp"
#include "particle_initialization.hpp"
#include "particle_mover.hpp"
#include "track_particle.hpp"

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
  // Get start time
  auto start = std::chrono::high_resolution_clock::now();

  // Parse inputs
  input::Input input = input::ParseArguments(argc, argv);

  // Print input
  std::cout << "Input file   : " << input.input_file() << std::endl;
  std::cout << "Problem name : " << input.problem_name() << std::endl;
  std::cout << "Input type   : " << input.input_type() << std::endl;

  // Print input mesh information
  mesh::PrintMeshSize(input.mesh());

  // Print input particle information
  std::cout << "==== Particle information ====" << std::endl;
  for (input::InputParticle particle : input.particles()) {
    std::cout << "* " << particle.name << std::endl;
    std::cout << "  n   = " << particle.n << std::endl;
    std::cout << "  m   = " << particle.m << std::endl;
    std::cout << "  q   = " << particle.q << std::endl;
    std::cout << "  tau = " << particle.tau << std::endl;
  }

  // Initialize particles
  int n_kind = input.particles().size();
  std::vector<particle::Particles> particles;
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    particles.push_back(particle::Particles(input.particles()[i_kind]));
  }
  particle::DistributeLocationUniform(particles[0], 0, 0.0, 1.0, 0.0, 1.0, 0.0,
                                      0.0);
  // particle::GammaTable gamma_table =
  //     particle::GTMaxwellian3D(input.particles()[0].tau);
  // particle::DistributeVelocityUniform(particles[0], 0, gamma_table);

  // Set some particles to be tracked
  int n_track = 10;
  int n_dump = 7;
  std::vector<int> track_ids(n_track);
  for (int i_track = 0; i_track < n_track; ++i_track) {
    particles[0].status()[i_track] = particle::ParticleStatus::Tracked;

    track_ids[i_track] = particles[0].id()[i_track];
  }
  particle::TrackParticle track_particle(n_track, n_dump);

  // Time loop
  int n_loop = 10;
  for (int i_loop = 0; i_loop < n_loop; ++i_loop) {
    track_particle.SaveTrackedParticles(particles[0]);

    // Move particles
    for (int i_p = 0; i_p < particles[0].npar(); ++i_p) {
      particles[0].x()[i_p] += 0.1;
      particles[0].y()[i_p] += 0.1;
      particles[0].z()[i_p] += 0.1;
    }
  }

  // Initialize field
  mesh::Field field(input.mesh());

  // Test ParticleMover
  particle::ParticleMover mover;
  mover.InitializeMover(input);
  std::cout << "Particle mover type: " << mover.type() << std::endl;
  std::cout << "Particle mover dt  : " << mover.dt() << std::endl;

  // // Sleep for 1 second
  // std::this_thread::sleep_for(std::chrono::seconds(1));

  // Print elapsed time
  std::cout << "Elapsed time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now() - start)
                   .count()
            << " ms" << std::endl;

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
