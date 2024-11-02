/**
 * @file itask_fields.cpp
 * @brief Source file for field initialization routines
 */
#include "fields.hpp"
#include "itask_fields.hpp"

namespace lili::task {
void TaskInitFields::Initialize() {
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

  // Call the base class Initialize
  Task::Initialize();
}
}  // namespace lili::task
