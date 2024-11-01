/**
 * @file task.cpp
 * @brief Source file for the Task related routines
 */

#include "task.hpp"

#include "task_init_particles.hpp"

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
    case TaskType::InitParticles:
      dynamic_cast<TaskInitParticles*>(task)->Execute();
      break;
    default:
      break;
  }
}

void ParseTaskList(input::Input& input) {
  // Add the create output folder task
  if (lili::rank == 0) {
    init_task_list.push_back(std::make_unique<TaskCreateOutput>());
  }

  // Add the particle initialization task
  init_task_list.push_back(std::make_unique<TaskInitParticles>(input));

  // Add the field initialization task

  // Add the field initialization task
  // init_task_list.push_back(std::make_unique<TaskInitFields>(input));
}
}  // namespace lili::task
