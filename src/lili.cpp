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
#include "ltask_pmove.hpp"
#include "mesh.hpp"
#include "output.hpp"
#include "parameter.hpp"
#include "particle.hpp"
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

  // Setup the output stream
  if (lili::rank != 0) lili::lout.enabled = false;

  lili::lout << "############ Initialization ############" << std::endl;
  lili::lout << "MPI size      : " << lili::nproc << std::endl;

  // Get start time
  auto start = std::chrono::high_resolution_clock::now();

  // == Read input =============================================================

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
  // Initialize the loop tasks
  for (auto& task : lili::task::loop_task_list) {
    lili::task::InitializeTask(task.get());
  }

  // Get the variable from the simulation variables
  std::vector<lili::particle::Particles>& particles =
      *std::get<std::unique_ptr<std::vector<lili::particle::Particles>>>(
           lili::task::sim_vars[lili::task::SimVarType::ParticlesVector])
           .get();
  std::vector<lili::particle::TrackParticles>& track_particles =
      *std::get<std::unique_ptr<std::vector<lili::particle::TrackParticles>>>(
           lili::task::sim_vars[lili::task::SimVarType::TrackParticlesVector])
           .get();
  lili::mesh::Fields& fields =
      *std::get<std::unique_ptr<lili::mesh::Fields>>(
           lili::task::sim_vars[lili::task::SimVarType::EMFields])
           .get();

  // // Compare the particles and track particles to what is in the task
  // //
  // // @todo Remove this after testing
  // for (auto& task : lili::task::init_task_list) {
  //   if (task->type() == lili::task::TaskType::InitParticles) {
  //     // Cast task to TaskInitParticles
  //     lili::task::TaskInitParticles& init_particles_task =
  //         dynamic_cast<lili::task::TaskInitParticles&>(*task);
  //     // Compare the particle pointers
  //     if (&particles != &init_particles_task.particles()) {
  //       lili::lout << "Particles pointer mismatch" << std::endl;
  //       lili::lout << "  Task particles pointer: "
  //                  << &init_particles_task.particles() << std::endl;
  //       lili::lout << "  Global particles pointer: " << &particles <<
  //       std::endl;
  //     } else {
  //       lili::lout << "Particles pointer match" << std::endl;
  //     }
  //     // Compare the track particles pointers
  //     if (&track_particles != &init_particles_task.track_particles()) {
  //       lili::lout << "Track particles pointer mismatch" << std::endl;
  //       lili::lout << "  Task track particles pointer: "
  //                  << &init_particles_task.track_particles() << std::endl;
  //       lili::lout << "  Global track particles pointer: " <<
  //       &track_particles
  //                  << std::endl;
  //     } else {
  //       lili::lout << "Track particles pointer match" << std::endl;
  //     }
  //   }
  // }

  // Temporarily use variables
  particles = particles;
  track_particles = track_particles;

  // Print Field information
  lili::mesh::PrintMeshSize(fields.size, lili::lout);

  lili::output_folder = lili::output_folder;
  MPI_Barrier(MPI_COMM_WORLD);

  // == Main loop =============================================================

  const int n_loop = input.loop().n_loop;
  const int nl_time = n_loop < 10000 ? n_loop : 10000;

  auto loop_time = std::chrono::high_resolution_clock::now();

  // Print time before the main loop
  lili::lout << "Initialization time: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(
                    loop_time - start)
                    .count()
             << " ms" << std::endl;
  MPI_Barrier(MPI_COMM_WORLD);

  lili::lout << "################# Loop #################" << std::endl;
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

    // Loop through all loop tasks
    for (auto& task : lili::task::loop_task_list) {
      lili::task::ExecuteTask(task.get());
    }

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

  // == Clean Up ===============================================================

  lili::lout << "############### Clean Up ###############" << std::endl;

  // Clean up the loop tasks
  for (auto& task : lili::task::loop_task_list) {
    lili::task::CleanUpTask(task.get());
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
