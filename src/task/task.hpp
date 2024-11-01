/**
 * @file task.hpp
 * @brief Header file for the Task related classes
 */
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "fields.hpp"
#include "input.hpp"
#include "parameter.hpp"
#include "particle.hpp"
#include "track_particle.hpp"

/**
 * @brief Namespace for LILI task related routines
 */
namespace lili::task {

/**
 * @brief Enumeration for task type
 */
enum class TaskType {
  Base,               ///< Base task
  CreateOutput,       ///< Task to create output folder
  InitParticles,      ///< Task to initialize particles
  InitFields,         ///< Task to initialize fields
  MoveParticlesFull,  ///< Task to move particles a full step
};

/**
 * @brief Enumeration for simulation variable type
 */
enum class SimVarType {
  EMFields,              ///< Electromagnetic Field object
  ParticlesVector,       ///< Vector of Particles object
  TrackParticlesVector,  ///< Vector of TrackParticles object
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
   * @brief Default task initialization function
   */
  virtual void Initialize() { is_init_ = true; }

  /**
   * @brief Default task execution function
   */
  virtual void Execute() {
    // Increment the run counter
    IncrementRun();
  }

  /**
   * @brief Default task for cleaning up
   */
  virtual void CleanUp() { is_cleaned_ = true; }

 private:
  TaskType type_;     ///< Type of the task
  std::string name_;  ///< Name of the task
  bool is_init_ = false;  ///< Flag to check if the task has been initialized
  int i_run_ = 0;     ///< Number of times the task has been run
  bool is_cleaned_ = false;  ///< Flag to check if the task has been cleaned up
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
    }
    std::cout << "Output folder : " << output_folder << std::endl;

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
 * @brief Map to store pointer to simulation variables
 */
extern std::map<
    task::SimVarType,
    std::variant<std::unique_ptr<mesh::Fields>,
                 std::unique_ptr<std::vector<particle::Particles>>,
                 std::unique_ptr<std::vector<particle::TrackParticles>>>>
    sim_vars;

/**
 * @brief Helper function to initialize the task object based on its type
 *
 * @param task Task object
 */
void InitializeTask(Task* task);

/**
 * @brief Helper function to execute the task object based on its type
 *
 * @param task Task object
 */
void ExecuteTask(Task* task);

/**
 * @brief Helper function to clean up the task object based on its type
 *
 * @param task Task object
 */
void CleanUpTask(Task* task);

/**
 * @brief Function to parse the task list from the input file
 *
 * @param input Input object
 */
void ParseTaskList(input::Input& input);
}  // namespace lili::task
