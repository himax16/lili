/**
 * @file task.cpp
 * @brief Source file for the Task related routines
 */

#include "task.hpp"

namespace lili {
// Initialize the
}
namespace lili::task {
// Initialize global variables
std::vector<std::unique_ptr<Task>> init_task_list;
std::vector<std::unique_ptr<Task>> loop_task_list;
std::map<SimVarType,
         std::variant<std::unique_ptr<mesh::Fields>,
                      std::unique_ptr<std::vector<particle::Particles>>,
                      std::unique_ptr<std::vector<particle::TrackParticles>>>>
    sim_vars;

void ExecuteTask(Task* task) {
  // Try to cast the task to the derived class
  switch (task->type()) {
    case TaskType::Base:
      task->Execute();
      break;
    case TaskType::CreateOutput:
      dynamic_cast<TaskCreateOutput*>(task)->Execute();
      break;
    default:
      break;
  }
}

void ParseTaskList(input::Input& input) {
  // Use input
  input = input;

  // Add the create output folder task
  if (lili::rank == 0) {
    init_task_list.push_back(std::make_unique<TaskCreateOutput>());
  }
}
}  // namespace lili::task
