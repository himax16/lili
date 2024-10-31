/**
 * @file task.hpp
 * @brief Header file for the Task related classes
 */
#pragma once

#include <string>
#include <vector>

#include "input.hpp"

/**
 * @brief Namespace for LILI task related routines
 */
namespace lili::task {
// Global variables
/**
 * @brief Simulation task list
 */
extern std::vector<Task> task_list;

/**
 * @brief Class to store simulation task information
 */
class Task {
 public:
  // Constructor
  Task(std::string name) : name_(name) {}

  // Getters
  /// @cond GETTERS
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
   * @brief Execute the task defined by the class
   */
  void Execute() {
    // Increment the run counter
    i_run_++;
  }

 private:
  int i_run_;
  std::string name_;
};

/**
 * @brief Function to parse the task list from the input file
 *
 * @param input Input object
 */
void ParseTaskList(input::Input& input);
}  // namespace lili::task
