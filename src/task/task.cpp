/**
 * @file task.cpp
 * @brief Source file for the Task related routines
 */

#include "task.hpp"

#include "itask_fields.hpp"
#include "itask_particles.hpp"
#include "ltask_pmove.hpp"

namespace lili::task {
// Initialize global variables
std::vector<std::unique_ptr<Task>> init_task_list;
std::vector<std::unique_ptr<Task>> loop_task_list;
std::map<SimVarType,
         std::variant<std::unique_ptr<mesh::Fields>,
                      std::unique_ptr<std::vector<particle::Particles>>,
                      std::unique_ptr<std::vector<particle::TrackParticles>>>>
    sim_vars;

void InitializeTask(Task* task) {
  // Try to cast the task to the derived class
  switch (task->type()) {
    case TaskType::MoveParticlesFull:
      dynamic_cast<TaskMoveParticlesFull*>(task)->Initialize();
      break;
    default:
      task->Initialize();
      break;
  }
}

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
    case TaskType::InitFields:
      dynamic_cast<TaskInitFields*>(task)->Execute();
      break;
    case TaskType::MoveParticlesFull:
      dynamic_cast<TaskMoveParticlesFull*>(task)->Execute();
      break;
    default:
      break;
  }
}

void CleanUpTask(Task* task) {
  // Try to cast the task to the derived class
  switch (task->type()) {
    default:
      task->CleanUp();
      break;
  }
}

void ParseTaskList(input::Input& input) {
  // Initialization tasks
  // Add the create output folder task
  if (lili::rank == 0) {
    init_task_list.push_back(std::make_unique<TaskCreateOutput>());
  }

  // Add the particle initialization task
  init_task_list.push_back(std::make_unique<TaskInitParticles>(input));

  // Add the field initialization task
  init_task_list.push_back(std::make_unique<TaskInitFields>(input));

  // Loop tasks
  for (auto& task : input.loop().tasks) {
    // Flag to check if the task is found
    bool task_found = false;

    // Switch based on the task name
    if (task.name == "move_particles") {
      // Check the type of the task
      if (task.type == "full") {
        loop_task_list.push_back(
            std::make_unique<TaskMoveParticlesFull>(input));
        task_found = true;
      }
    }

    // Check if the task is found
    if (!task_found) {
      lili::lout << "Task not found: " << task.name << std::endl;
    }
  }
  // Print the task list
  lili::lout << "=========== Task information ===========" << std::endl;
  lili::lout << "Initialization tasks: " << std::endl;
  for (auto& task : init_task_list) {
    lili::lout << "  Name        : " << task->name() << std::endl;
  }
  lili::lout << "Loop tasks: " << std::endl;
  for (auto& task : loop_task_list) {
    lili::lout << "  Name        : " << task->name() << std::endl;
  }
}
}  // namespace lili::task
