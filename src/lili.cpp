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

  // Execute the default tasks
  for (auto& task : lili::task::default_task_list) {
    lili::task::InitializeTask(task.get());
  }
  // Initialize the loop tasks
  for (auto& task : lili::task::loop_task_list) {
    lili::task::InitializeTask(task.get());
  }

  // Check if all tasks are initialized
  for (auto& task : lili::task::default_task_list) {
    if (!task->is_initialized()) {
      lili::lout << "Task not initialized: " << task->name() << std::endl;
    }
  }
  for (auto& task : lili::task::loop_task_list) {
    if (!task->is_initialized()) {
      lili::lout << "Task not initialized: " << task->name() << std::endl;
    }
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

  // Temporarily use variables
  particles = particles;
  track_particles = track_particles;

  // Print Field information
  lili::mesh::PrintMeshSize(fields.size, lili::lout);
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
    // Loop through all default tasks
    for (auto& task : lili::task::default_task_list) {
      lili::task::ExecuteTask(task.get());
    }

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

  // Clean up the default tasks
  for (auto& task : lili::task::default_task_list) {
    lili::task::CleanUpTask(task.get());
  }

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
