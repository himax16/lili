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
  mesh::LoadFieldTo(field, "tpf_09_8000.h5", false);
  // field.ex += 1.0e-7;

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
  std::vector<particle::TrackParticles> track_particles(n_kind);
  std::vector<int> dl_tracks(n_kind);
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    particles[i_kind] = particle::Particles(input.particles()[i_kind]);
    particle::DistributeID(particles[i_kind], 0);

    // Distribute positions
    // particle::DistributeLocationUniform(particles[i_kind], 0, input.mesh());

    // Distribute velocities
    particle::GammaTable gamma_table =
        particle::GTMaxwellian3D(input.particles()[i_kind].tau);
    particle::DistributeVelocityUniform(particles[i_kind], 0, gamma_table);
    // particle::AddBulkVelocity(particles[i_kind], 0.4, 0.0, 0.0);

    // Add tracking
    int n_track = input.particles()[i_kind].n_track;
    dl_tracks[i_kind] = input.particles()[i_kind].dl_track;
    n_track = std::min(n_track, particles[i_kind].npar());
    track_particles[i_kind] = particle::TrackParticles(
        n_track, input.particles()[i_kind].dtrack_save);
    track_particles[i_kind].SetPrefix(std::filesystem::path(output_folder) /
                                      ("tp_" + input.particles()[i_kind].name));

    // Assign some particle to be tracked
    for (int i_track = 0; i_track < n_track; ++i_track) {
      particles[i_kind].status(i_track) = particle::ParticleStatus::Tracked;
    }
  }

  // Initialize particle mover
  particle ::ParticleMover mover;
  mover.InitializeMover(input.integrator());
  std::cout << "Particle mover type: " << mover.type() << std::endl;
  std::cout << "Particle mover dt  : " << mover.dt() << std::endl;

  // Time loop
  int n_loop = input.integrator().n_loop;
  for (int i_loop = 0; i_loop < n_loop; ++i_loop) {
    // Loop through all particles
    for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
      // Save tracked particles if needed
      if (track_particles[i_kind].ntrack() > 0) {
        if (i_loop % dl_tracks[i_kind] == 0)
          track_particles[i_kind].SaveTrackedParticles(particles[i_kind]);
      }

      // Move particles
      mover.Move(particles[i_kind], field);
      particle::PeriodicBoundaryParticles(particles[i_kind], input.mesh());
    }
  }

  // Save and dump tracked particles at the end
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    if (track_particles[i_kind].ntrack() > 0) {
      track_particles[i_kind].SaveTrackedParticles(particles[i_kind]);
      track_particles[i_kind].DumpTrackedParticles();
    }
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
