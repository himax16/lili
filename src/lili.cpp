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
output::LiliCout lout;
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

  if (lili::rank != 0) lili::lout.enabled = false;
  lili::lout << "MPI initialized with size of: " << lili::nproc << std::endl;

  // Get start time
  auto start = std::chrono::high_resolution_clock::now();
  // == Read input ============================================================
  // Parse inputs
  lili::input::Input input =
      lili::input::ParseArguments(argc, argv, lili::lout);
  // Print the input and input mesh information
  input.Print(lili::lout);

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
    lili::lout << "Task: " << task->name() << " executed " << task->i_run()
               << " times" << std::endl;
  }

  // Get the variable from the simulation variables
  std::vector<lili::particle::Particles>& particles_t =
      *std::get<std::unique_ptr<std::vector<lili::particle::Particles>>>(
           lili::task::sim_vars[lili::task::SimVarType::ParticlesVector])
           .get();

  // Print the length
  lili::lout << "Number of particles: " << particles_t.size() << std::endl;
  // Iterate over the particles and print their status
  for (auto& particles : particles_t) {
    for (int i = 0; i < particles.npar(); ++i) {
      if (particles.status(i) == lili::particle::ParticleStatus::Tracked)
        lili::lout << "Particle " << particles.id(i) << " status: "
                   << "Tracked" << std::endl;
    }
  }

  MPI_Abort(MPI_COMM_WORLD, 0);

  lili::output_folder = lili::output_folder;
  MPI_Barrier(MPI_COMM_WORLD);

  // Initialize fields
  lili::mesh::Fields fields(input.mesh());
  switch (input.input_type()) {
    case lili::input::InputType::Initial:
      break;

    case lili::input::InputType::Restart:
      lili::lout << "Loading fields data from: " << input.restart_file()
                 << std::endl;
      lili::mesh::LoadFieldTo(fields, input.restart_file().c_str(), false);
      break;

    case lili::input::InputType::TestParticle:
      lili::lout << "Loading fields data from: " << input.restart_file()
                 << std::endl;
      lili::mesh::LoadFieldTo(fields, input.restart_file().c_str(), false);
      break;

    default:
      break;
  }

  // Initialize particle mover
  lili::particle ::ParticleMover mover;
  mover.InitializeMover(input.integrator());
  lili::lout << "Particle mover type: " << mover.type() << std::endl;
  lili::lout << "Particle mover dt  : " << mover.dt() << std::endl;

  // == Main loop =============================================================
  const int n_loop = input.integrator().n_loop;
  const int nl_time = n_loop < 10000 ? n_loop : 10000;

  auto loop_time = std::chrono::high_resolution_clock::now();

  // Print time before the main loop
  lili::lout << "Initialization time: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(
                    loop_time - start)
                    .count()
             << " ms" << std::endl;
  MPI_Barrier(MPI_COMM_WORLD);

  for (int i_loop = 0; i_loop < n_loop; ++i_loop) {
    // // Loop through all particles
    // for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
    //   // Save tracked particles if needed
    //   if (track_particles[i_kind].n_track() > 0) {
    //     if (i_loop % dl_tracks[i_kind] == 0)
    //       track_particles[i_kind].SaveTrackedParticles(particles[i_kind],
    //                                                    fields);
    //   }

    //   // Move particles
    //   mover.Move(particles[i_kind], fields);
    //   lili::particle::PeriodicBoundaryParticles(particles[i_kind],
    //                                             input.mesh());
    // }

    // Print loop information
    if (i_loop % nl_time == 0) {
      lili::lout << "Iteration: " << i_loop << " / " << n_loop;
      // Print timing
      lili::lout << " ("
                 << std::chrono::duration_cast<std::chrono::microseconds>(
                        (std::chrono::high_resolution_clock::now() -
                         loop_time) /
                        nl_time)
                        .count()
                 << " us / loop)" << std::endl;
      loop_time = std::chrono::high_resolution_clock::now();
    }
  }

  // // Save and dump tracked particles at the end
  // for (int i_kind = 0; i_kind < n_kind; ++i_kind) {
  //   if (track_particles[i_kind].n_track() > 0) {
  //     track_particles[i_kind].SaveTrackedParticles(particles[i_kind],
  //     fields); track_particles[i_kind].DumpTrackedParticles();
  //   }
  // }

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
