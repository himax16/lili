/**
 * @file itask_fields.hpp
 * @brief Header file for field initialization routines
 */
#pragma once

#include <string>

#include "input.hpp"
#include "mesh.hpp"
#include "task.hpp"

namespace lili::task {
/**
 * @brief Task class to initialize Fields
 */
class TaskInitFields : public Task {
 public:
  // Constructor
  TaskInitFields() : Task(TaskType::InitFields) { set_name("InitFields"); }
  TaskInitFields(input::Input input) : Task(TaskType::InitFields) {
    set_name("InitFields");

    // Copy the mesh size
    mesh_size_ = input.mesh();

    // Check if we need to read the fields from file
    input::InputType input_type = input.input_type();
    if (input_type == input::InputType::Restart ||
        input_type == input::InputType::TestParticle) {
      from_file_ = true;
      restart_file_ = input.restart_file();
    } else {
      from_file_ = false;
    }
  }

  /**
   * @brief Initialize particles
   */
  void Execute() override;

 private:
  mesh::MeshSize mesh_size_;  ///< Fields data mesh size
  bool from_file_;            ///< Whether Fields are read from file
  std::string restart_file_;  ///< Restart file name
};
}  // namespace lili::task
