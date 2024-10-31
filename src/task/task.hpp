/**
 * @file task.hpp
 * @brief Header file for the Task related classes
 */
#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "input.hpp"
#include "parameter.hpp"

/**
 * @brief Namespace for LILI task related routines
 */
namespace lili::task {

/**
 * @brief Enumeration for task type
 */
enum class TaskType {
  Base,
  CreateOutput,
  InitParticles,
};

/**
 * @brief Base class to store simulation task information
 */
class Task {
 public:
  // Constructor
  Task() : type_(TaskType::Base), name_("") {}
  Task(TaskType type) : type_(type), name_("") {}
  Task(std::string name) : type_(TaskType::Base), name_(name) {}

  // Getters
  /// @cond GETTERS
  /**
   * @brief Get the type of the task
   *
   * @return TaskType Type of the task
   */
  TaskType type() const { return type_; }

  /**
   * @brief Get the name of the task
   *
   * @return std::string Name of the task
   */
  std::string name() const { return name_; }

  /**
   * @brief Get the number of times the task has been run
   *
   * @return int Number of times the task has been run
   */
  int i_run() const { return i_run_; }
  /// @endcond

  // Setters
  /// @cond SETTERS
  /**
   * @brief Set the name of the task
   *
   * @param name Name of the task
   */
  void set_name(std::string name) { name_ = name; }
  /// @endcond

  /**
   * @brief Increase the run counter by one
   */
  int IncrementRun() { return i_run_++; }

  /**
   * @brief Default task execution function
   */
  virtual void Execute() {
    std::cout << "BaseTask: " << name_ << std::endl;
    // Increment the run counter
    IncrementRun();
  }

 private:
  TaskType type_;     ///< Type of the task
  int i_run_ = 0;     ///< Number of times the task has been run
  std::string name_;  ///< Name of the task
};

/**
 * @brief Task class to create output folder
 */
class TaskCreateOutput : public Task {
 public:
  // Constructor
  TaskCreateOutput() : Task(TaskType::CreateOutput) {
    set_name("CreateOutput");
  }
  TaskCreateOutput(std::string output_folder) : Task(TaskType::CreateOutput) {
    lili::output_folder = output_folder;
    set_name("CreateOutput");
  }

  /**
   * @brief Create simulation output folder
   */
  void Execute() override {
    std::string output_folder = lili::output_folder;
    if (!std::filesystem::is_directory(output_folder)) {
      std::filesystem::create_directory(output_folder);
      std::cout << "Created output folder: " << output_folder << std::endl;
    } else {
      std::cout << "Output folder        : " << output_folder << std::endl;
    }

    // Increment the run counter
    IncrementRun();
  }
};

// Global variables
/**
 * @brief Simulation initialization task list
 */
extern std::vector<std::unique_ptr<Task>> init_task_list;
/**
 * @brief Simulation loop task list
 */
extern std::vector<std::unique_ptr<Task>> loop_task_list;

/**
 * @brief Helper function to execute the task object based on its type
 *
 * @param task Task object
 */
void ExecuteTask(Task* task);

/**
 * @brief Function to parse the task list from the input file
 *
 * @param input Input object
 */
void ParseTaskList(input::Input& input);
}  // namespace lili::task
