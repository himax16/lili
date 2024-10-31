/**
 * @file lili.cpp
 * @brief Main LILI program
 */
#include <chrono>
#include <cmath>
#include <filesystem>
#include <iomanip>
#include <thread>

#include "fields.hpp"
#include "hdf5.h"
#include "input.hpp"
#include "mesh.hpp"
#include "output.hpp"
#include "parameter.hpp"
#include "particle.hpp"
#include "particle_initialization.hpp"
#include "particle_mover.hpp"
#include "task.hpp"
#include "track_particle.hpp"

/**
 * @brief Base namespace for LILI program
 *
 * @details
 * The base namespace for the LILI program also contains all of the global
 * variables and functions.
 */
namespace lili {
int rank, nproc;
std::string output_folder = "output";
}  // namespace lili

/**
 * @brief Main function for `LILI` program
 */
int main(int argc, char* argv[]) {
  // == Pre-initialization =====================================================
  // MPI initialization
  // @todo Move this into header only file using inline
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &lili::rank);
  MPI_Comm_size(MPI_COMM_WORLD, &lili::nproc);

  // Create LiliCout object for output
  lili::output::LiliCout lout;
  if (lili::rank != 0) lout.enabled = false;
  lout << "MPI initialized with size of: " << lili::nproc << std::endl;

  // Get start time
  auto start = std::chrono::high_resolution_clock::now();
  // == Read input ============================================================
  // Parse inputs
  lili::input::Input input = lili::input::ParseArguments(argc, argv, lout);
  // Print the input and input mesh information
  input.Print(lout);

  MPI_Barrier(MPI_COMM_WORLD);
  // == Initialization =========================================================
  // Parse the tasks from input
  lili::task::ParseTaskList(input);

  // Execute the initialization tasks
  for (auto& task : lili::task::init_task_list) {
    lili::task::ExecuteTask(task.get());
  }
  // Print the execution number of the task
  for (auto& task : lili::task::init_task_list) {
    lout << "Task: " << task->name() << " executed " << task->i_run()
         << " times" << std::endl;
  }
  std::cout << "Rank " << lili::rank
            << " init: " << lili::task::init_task_list.size() << std::endl;

  std::string output_folder = lili::output_folder;
  MPI_Barrier(MPI_COMM_WORLD);

  // Initialize fields
  lili::mesh::Fields fields(input.mesh());
  switch (input.input_type()) {
    case lili::input::InputType::Initial:
      break;

    case lili::input::InputType::Restart:
      lout << "Loading fields data from: " << input.restart_file() << std::endl;
      lili::mesh::LoadFieldTo(fields, input.restart_file().c_str(), false);
      break;

    case lili::input::InputType::TestParticle:
      lout << "Loading fields data from: " << input.restart_file() << std::endl;
      lili::mesh::LoadFieldTo(fields, input.restart_file().c_str(), false);
      break;

    default:
      break;
  }

  // Get the particle information
  int n_kind = input.particles().size();
  std::vector<int> n_particles;
  for (lili::input::InputParticle particle : input.particles()) {
    n_particles.push_back(particle.n);
  }

  // Print particle information if available
  lout << "==== Particle information ====" << std::endl;
  for (lili::input::InputParticle particle : input.particles()) {
    lout << "* " << particle.name << std::endl;
    lout << "  n   = " << particle.n << std::endl;
    lout << "  m   = " << particle.m << std::endl;
    lout << "  q   = " << particle.q << std::endl;
    lout << "  tau = " << particle.tau << std::endl;
  }

  // Initialize particles
  std::vector<lili::particle::Particles> particles(n_kind);
  std::vector<lili::particle::TrackParticles> track_particles(n_kind);
  std::vector<int> dl_tracks(n_kind);
  for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    particles[i_kind] = lili::particle::Particles(input.particles()[i_kind]);
    lili::particle::DistributeID(particles[i_kind],
                                 lili::rank * input.particles()[i_kind].n);

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
  lout << "Particle mover type: " << mover.type() << std::endl;
  lout << "Particle mover dt  : " << mover.dt() << std::endl;

  // == Main loop =============================================================
  const int n_loop = input.integrator().n_loop;
  const int nl_time = n_loop < 10000 ? n_loop : 10000;

  auto loop_time = std::chrono::high_resolution_clock::now();

  // Print time before the main loop
  lout << "Initialization time: "
       << std::chrono::duration_cast<std::chrono::milliseconds>(loop_time -
                                                                start)
              .count()
       << " ms" << std::endl;
  MPI_Barrier(MPI_COMM_WORLD);

  for (int i_loop = 0; i_loop < n_loop; ++i_loop) {
    // Loop through all particles
    for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
      // Save tracked particles if needed
      if (track_particles[i_kind].ntrack() > 0) {
        if (i_loop % dl_tracks[i_kind] == 0)
          track_particles[i_kind].SaveTrackedParticles(particles[i_kind],
                                                       fields);
      }

      // Move particles
      mover.Move(particles[i_kind], fields);
      lili::particle::PeriodicBoundaryParticles(particles[i_kind],
                                                input.mesh());
    }

    // Print loop information
    if (i_loop % nl_time == 0) {
      lout << "Iteration: " << i_loop << " / " << n_loop;
      // Print timing
      lout << " ("
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
      track_particles[i_kind].SaveTrackedParticles(particles[i_kind], fields);
      track_particles[i_kind].DumpTrackedParticles();
    }
  }

  // Print elapsed time
  std ::cout << "Elapsed time: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start)
                    .count()
             << " ms" << std::endl;

  // MPI finalize
  MPI_Finalize();

  return 0;
}
