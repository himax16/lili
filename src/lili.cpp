/**
 * @file lili.cpp
 * @brief Main LILI program
 */
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

/**
 * @brief Main function for `LILI` program
 */
int main(int argc, char* argv[]) {
  // MPI initialization
  // @todo Move this into header only file using inline
  int rank, nproc;
  rank = 0;
  nproc = 1;
  if (rank == 0) {
    std::cout << "MPI initialized with size of: " << nproc << std::endl;
  }

  // Get start time
  auto start = std::chrono::high_resolution_clock::now();

  // Parse inputs
  lili::input::Input input = lili::input::ParseArguments(argc, argv);

  // Print input
  std::cout << "Input file   : " << input.input_file() << std::endl;
  std::cout << "Problem name : " << input.problem_name() << std::endl;
  std::cout << "Input type   : "
            << lili::input::InputTypeToString(input.input_type()) << std::endl;

  // Initialize output folder
  std::string output_folder = "output";
  if (!std::filesystem::is_directory(output_folder)) {
    std::filesystem::create_directory(output_folder);
    std::cout << "Created output folder: " << output_folder << std::endl;
  } else {
    std::cout << "Output folder: " << output_folder << std::endl;
  }

  // Print input mesh information
  lili::mesh::PrintMeshSize(input.mesh());

  // Initialize field
  lili::mesh::Field field(input.mesh());
  switch (input.input_type()) {
    case lili::input::InputType::Initial:
      break;

    case lili::input::InputType::Restart:
      std::cout << "Loading field data from: " << input.restart_file()
                << std::endl;
      lili::mesh::LoadFieldTo(field, input.restart_file().c_str(), false);
      break;

    case lili::input::InputType::TestParticle:
      std::cout << "Loading field data from: " << input.restart_file()
                << std::endl;
      lili::mesh::LoadFieldTo(field, input.restart_file().c_str(), false);
      break;

    default:
      break;
  }

  // Print input particle information
  std::cout << "==== Particle information ====" << std::endl;
  for (lili::input::InputParticle particle : input.particles()) {
    std::cout << "* " << particle.name << std::endl;
    std::cout << "  n   = " << particle.n << std::endl;
    std::cout << "  m   = " << particle.m << std::endl;
    std::cout << "  q   = " << particle.q << std::endl;
    std::cout << "  tau = " << particle.tau << std::endl;
  }

  // Initialize particles
  int n_kind = input.particles().size();
  std::vector<lili::particle::Particles> particles(n_kind);
  std::vector<lili::particle::TrackParticles> track_particles(n_kind);
  std::vector<int> dl_tracks(n_kind);
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    particles[i_kind] = lili::particle::Particles(input.particles()[i_kind]);
    lili::particle::DistributeID(particles[i_kind],
                                 rank * input.particles()[i_kind].n);

    // Distribute positions
    // particle::DistributeLocationUniform(particles[i_kind], 0, input.mesh());

    // Distribute velocities
    lili::particle::GammaTable gamma_table =
        lili::particle::GTMaxwellian3D(input.particles()[i_kind].tau);
    lili::particle::DistributeVelocityUniform(particles[i_kind], 0,
                                              gamma_table);
    // particle::AddBulkVelocity(particles[i_kind], 0.4, 0.0, 0.0);

    // Add tracking
    int n_track = input.particles()[i_kind].n_track;
    dl_tracks[i_kind] = input.particles()[i_kind].dl_track;
    n_track = std::min(n_track, particles[i_kind].npar());
    track_particles[i_kind] = lili::particle::TrackParticles(
        n_track, input.particles()[i_kind].dtrack_save);
    track_particles[i_kind].SetPrefix(std::filesystem::path(output_folder) /
                                      ("tp_" + input.particles()[i_kind].name));

    // Assign some particle to be tracked
    for (int i_track = 0; i_track < n_track; ++i_track) {
      particles[i_kind].status(i_track) =
          lili::particle::ParticleStatus::Tracked;
    }
  }

  // Initialize particle mover
  lili::particle ::ParticleMover mover;
  mover.InitializeMover(input.integrator());
  std::cout << "Particle mover type: " << mover.type() << std::endl;
  std::cout << "Particle mover dt  : " << mover.dt() << std::endl;

  // Time loop
  const int n_loop = input.integrator().n_loop;
  //@todo Redefine this variable into global constant
  const int nl_time = 10000;
  auto loop_time = std::chrono::high_resolution_clock::now();
  for (int i_loop = 0; i_loop < n_loop; ++i_loop) {
    // Loop through all particles
    for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
      // Save tracked particles if needed
      if (track_particles[i_kind].ntrack() > 0) {
        if (i_loop % dl_tracks[i_kind] == 0)
          track_particles[i_kind].SaveTrackedParticles(particles[i_kind],
                                                       field);
      }

      // Move particles
      mover.Move(particles[i_kind], field);
      lili::particle::PeriodicBoundaryParticles(particles[i_kind],
                                                input.mesh());
    }

    // Print loop information
    if (i_loop % nl_time == 0) {
      std::cout << "Iteration: " << i_loop << " / " << n_loop;
      // Print timing
      std::cout << " ("
                << std::chrono::duration_cast<std::chrono::microseconds>(
                       (std::chrono::high_resolution_clock::now() - loop_time) /
                       nl_time)
                       .count()
                << " us / loop)" << std::endl;
      loop_time = std::chrono::high_resolution_clock::now();
    }
  }

  // Save and dump tracked particles at the end
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    if (track_particles[i_kind].ntrack() > 0) {
      track_particles[i_kind].SaveTrackedParticles(particles[i_kind], field);
      track_particles[i_kind].DumpTrackedParticles();
    }
  }

  // Print elapsed time
  std ::cout << "Elapsed time: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start)
                    .count()
             << " ms" << std::endl;

  return 0;
}
