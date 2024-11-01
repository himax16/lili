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
 * @brief Rate at which the loop prints the iteration information
 */
#define __LILI_LPRINT_RATE 10000

/**
 * @brief Base namespace for LILI program
 *
 * @details
 * The base namespace for the LILI program also contains all of the global
 * variables and functions.
 */
namespace lili {
// Initialize global variables
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

  // == Initialization =========================================================
  // Parse inputs
  lili::input::Input input =
      lili::input::ParseArguments(argc, argv, lili::lout);

  // Print the input and input mesh information
  input.Print(lili::lout);

  MPI_Barrier(MPI_COMM_WORLD);

  // Set the number of loop iterations
  const int n_loop = input.loop().n_loop;
  const int nl_time = n_loop < __LILI_LPRINT_RATE ? n_loop : __LILI_LPRINT_RATE;

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

  MPI_Barrier(MPI_COMM_WORLD);

  // == Main loop =============================================================
  lili::lout << "################# Loop #################" << std::endl;

  auto loop_time = std::chrono::high_resolution_clock::now();

  // Print time before the main loop
  lili::lout << "Initialization time: "
             << std::chrono::duration_cast<std::chrono::milliseconds>(
                    loop_time - start)
                    .count()
             << " ms" << std::endl;
  MPI_Barrier(MPI_COMM_WORLD);

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
