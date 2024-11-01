/**
 * @file task_init_fields.cpp
 * @brief Source file for field initialization routines
 */
#include "task_init_fields.hpp"

#include "fields.hpp"

namespace lili::task {
void TaskInitFields::Execute() {
  // Initialize Fields object
  lili::mesh::Fields fields(mesh_size_);

  // Load fields from file if needed
  if (from_file_) {
    lili::lout << "Loading fields data from: " << restart_file_ << std::endl;
    lili::mesh::LoadFieldTo(fields, restart_file_.c_str(), false);
  }

  // Store the fields in the simulation variables
  sim_vars[SimVarType::EMFields] =
      std::make_unique<lili::mesh::Fields>(std::move(fields));

  // Increment the run counter
  IncrementRun();
}
}  // namespace lili::task
