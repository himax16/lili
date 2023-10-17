/**
 * @file lili.cpp
 * @brief Main LILI program
 *
 */

// #include <numbers>

#include <chrono>
#include <cmath>
#include <filesystem>
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
int main(int argc, char* argv[]) {
  // Get start time
  auto start = std::chrono::high_resolution_clock::now();

  // Parse inputs
  input::Input input = input::ParseArguments(argc, argv);

  // Print input
  std::cout << "Input file   : " << input.input_file() << std::endl;
  std::cout << "Problem name : " << input.problem_name() << std::endl;
  std::cout << "Input type   : " << input.input_type() << std::endl;

  // Initialize output folder
  std::string output_folder = "output";
  if (!std::filesystem::is_directory(output_folder)) {
    std::filesystem::create_directory(output_folder);
    std::cout << "Created output folder: " << output_folder << std::endl;
  } else {
    std::cout << "Output folder: " << output_folder << std::endl;
  }

  // Print input mesh information
  mesh::PrintMeshSize(input.mesh());

  // Initialize field
  mesh::Field field(input.mesh());
  field.bz = 2.2627e-03;

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
  std::vector<particle::Particles> particles(n_kind);
  std::vector<particle::TrackParticles> track_particles;
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    particles[i_kind] = particle::Particles(input.particles()[i_kind]);
    particle::DistributeID(particles[i_kind], 0);

    // Distribute positions
    particle::DistributeLocationUniform(particles[i_kind], 0, input.mesh());

    // Distribute velocities
    particle::GammaTable gamma_table =
        particle::GTMaxwellian3D(input.particles()[i_kind].tau);
    particle::DistributeVelocityUniform(particles[i_kind], 0, gamma_table);
    // particle::AddBulkVelocity(particles[i_kind], 0.75, 0.0, 0.0);
  }

  // Set some particles to be tracked
  int n_track = 100000;
  int n_dump = 1000;
  std::vector<int> track_ids(n_track);
  for (int i_track = 0; i_track < n_track; ++i_track) {
    particles[0].status()[i_track] = particle::ParticleStatus::Tracked;

    track_ids[i_track] = particles[0].id()[i_track];
  }
  particle::TrackParticles track_particle(n_track, n_dump);
  track_particle.SetPrefix(std::filesystem::path(output_folder) / "tp");

  // Initialize particle mover
  particle ::ParticleMover mover;
  mover.InitializeMover(input);
  std::cout << "Particle mover type: " << mover.type() << std::endl;
  std::cout << "Particle mover dt  : " << mover.dt() << std::endl;

  // Time loop
  int n_loop = 10000;
  int n_rate = 1;

  for (int i_loop = 0; i_loop < n_loop; ++i_loop) {
    // Save tracked particle data
    if (i_loop % n_rate == 0) {
      track_particle.SaveTrackedParticles(particles[0]);
    }

    // Move particles
    for (particle::Particles& par : particles) {
      mover.Move(par, field);
      particle::PeriodicBoundaryParticles(par, input.mesh());
    }
  }

  // Save and dump tracked particles
  track_particle.SaveTrackedParticles(particles[0]);
  if (track_particle.itrack() > 0) {
    track_particle.DumpTrackedParticles();
  }

  // // Sleep for 1 second
  // std::this_thread::sleep_for(std::chrono::seconds(1));

  // Print elapsed time
  std ::cout << "Elapsed time: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start)
                    .count()
             << " ms" << std::endl;

  return 0;
}
