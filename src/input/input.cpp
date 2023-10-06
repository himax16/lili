/**
 * @file    input.cpp
 * @brief   Source file for processing CLI and user inputs
 */
#include "input.hpp"

#include "json.hpp"
#include "output.hpp"

// Simplify namespace
using json = nlohmann::json;

namespace lili::input {
/**
 * @brief Default constructor for Input class
 */
Input::Input() {
  input_file_ = "";
  problem_name_ = "LILI";
  input_type_ = InputType::None;
}

/**
 * @brief Constructor for Input class
 *
 * @param[in] in_file
 * Input file
 */
Input::Input(const char *in_file) {
  input_file_ = in_file;
  problem_name_ = "LILI";
  input_type_ = InputType::None;
}

/**
 * @brief Copy constructor for Input class
 * @param[in] input
 * Input object
 */
Input::Input(const Input &input) {
  input_file_ = input.input_file_;
  problem_name_ = input.problem_name_;
  input_type_ = input.input_type_;

  mesh_ = input.mesh_;
  particles_ = input.particles_;
}

/**
 * @brief Swap data from Input class
 *
 * @param[in] first
 * First Input object
 * @param[in] second
 * Second Input object
 */
void swap(Input &first, Input &second) {
  using std::swap;

  swap(first.input_file_, second.input_file_);
  swap(first.problem_name_, second.problem_name_);
  swap(first.input_type_, second.input_type_);

  swap(first.mesh_, second.mesh_);
  swap(first.particles_, second.particles_);
}

/**
 * @brief Parse input file
 *
 * @param[in] in_file
 * Input file
 */
void Input::Parse() {
  // Open input file
  std::ifstream ifs(input_file_.c_str());
  if (!ifs.is_open()) {
    std::cerr << "Cannot open input file: " << input_file_ << std::endl;
    exit(2);
  }

  // Parse the JSON file ignoring comment
  json j;
  try {
    j = json::parse(ifs,
                    /* callback */ nullptr,
                    /* allow exceptions */ true,
                    /* ignore comments */ true);
  } catch (json::parse_error &e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
    exit(2);
  }

  // Close input file
  ifs.close();

  // Check the type of input file
  if (!j.contains("input_type")) {
    std::cerr << "No input type in " << input_file_ << std::endl;
    exit(2);
  } else {
    std::string input_type_str = j.at("input_type").get<std::string>();
    if (strcmp(input_type_str.c_str(), "initial") == 0) {
      input_type_ = InputType::Initial;
    } else if (strcmp(input_type_str.c_str(), "restart") == 0) {
      input_type_ = InputType::Restart;
    } else {
      std::cerr << "Unrecognized input type in " << input_type_str << std::endl;
      std::cerr << "Available input type: [initial | restart]" << std::endl;
      exit(2);
    }
  }

  // Parse problem name
  if (!j.contains("problem_name")) {
    std::cerr << "No problem name in " << input_file_ << std::endl;
    std::cerr << "Using default problem name: LILI" << std::endl;
  } else {
    problem_name_ = j.at("problem_name").get<std::string>();
  }

  // Parse mesh
  if (j.contains("mesh")) {
    auto j_mesh = j.at("mesh");
    // Parse mesh dimension
    mesh_.dim = j_mesh.at("dimension").get<int>();
    if (mesh_.dim < 1 || mesh_.dim > 3) {
      std::cerr << "Invalid mesh dimension in " << input_file_ << std::endl;
      exit(2);
    }

    // Parse mesh size
    mesh_.nx = j_mesh.at("x").at("n").get<int>();
    mesh_.lx = j_mesh.at("x").at("l").get<double>();
    if (j_mesh.at("x").contains("ng")) {
      mesh_.ngx = j_mesh.at("x").at("ng").get<int>();
    } else {
      mesh_.ngx = __LILIM_DEFAULT_NGHOST;
    }

    if (mesh_.dim > 1) {
      mesh_.ny = j_mesh.at("y").at("n").get<int>();
      mesh_.ly = j_mesh.at("y").at("l").get<double>();
      if (j_mesh.at("y").contains("ng")) {
        mesh_.ngy = j_mesh.at("y").at("ng").get<int>();
      } else {
        mesh_.ngy = __LILIM_DEFAULT_NGHOST;
      }
    } else {
      mesh_.ny = 1;
      mesh_.ly = 1.0;
      mesh_.ngy = 0;
    }

    if (mesh_.dim > 2) {
      mesh_.nz = j_mesh.at("z").at("n").get<int>();
      mesh_.lz = j_mesh.at("z").at("l").get<double>();
      if (j_mesh.at("z").contains("ng")) {
        mesh_.ngz = j_mesh.at("z").at("ng").get<int>();
      } else {
        mesh_.ngz = __LILIM_DEFAULT_NGHOST;
      }
    } else {
      mesh_.nz = 1;
      mesh_.lz = 1.0;
      mesh_.ngz = 0;
    }
  }

  // Parse particles
  if (j.contains("particles")) {
    // Iterate over all particles
    for (auto &[key, val] : j.at("particles").items()) {
      InputParticle particle;
      particle.name = key;

      // Parse particle variables
      particle.n = val.at("n").get<int>();
      particle.m = val.at("m").get<double>();
      particle.q = val.at("q").get<double>();
      particle.tau = val.value("tau", 0.0);

      // Add particle to the list
      particles_.push_back(particle);
    }
  }

  // // serialization with pretty printing
  // // pass in the amount of spaces to indent
  // std::cout << j.dump(4) << std::endl;
}

/**
 * @brief Parse command line arguments
 *
 * @param[in] argc
 * Number of command line arguments
 * @param[in] argv
 * Command line arguments
 */
Input ParseArguments(int argc, char *argv[]) {
  // Variable declaration
  int i_arg = 0;
  bool has_input = false;
  Input input;

  // Parse command line arguments
  for (i_arg = 1; i_arg < argc; ++i_arg) {
    if (argv[i_arg][0] == '-') {
      switch (argv[i_arg][1]) {
        case '-':
          // Long option
          if (strcmp(argv[i_arg], "--help") == 0) {
            // Print help
            lili::output::PrintHelp();
            exit(0);
          } else if (strcmp(argv[i_arg], "--version") == 0) {
            // Print version
            lili::output::PrintVersion();
            exit(0);
          } else if (strcmp(argv[i_arg], "--input") == 0) {
            if (has_input) {
              // Throw error for multiple input file
              std::cerr << "There are multiple input file" << std::endl;
              exit(1);
            }
            // Parse input file
            input.input_file() = argv[++i_arg];
            input.Parse();
            has_input = true;
          } else if (strcmp(argv[i_arg], "--") == 0) {
            // End of options
            ++i_arg;
            break;
          } else {
            // Throw error for unrecognized option
            std::cerr << "Unrecognized option: " << argv[i_arg] << std::endl;
            exit(1);
          }
        case 'i':
          if (has_input) {
            // Throw error for multiple input file
            std::cerr << "There are multiple input file" << std::endl;
            exit(1);
          }
          // Parse input file
          input.input_file() = argv[++i_arg];
          input.Parse();
          has_input = true;
        case 'h':
          // Print help
          lili::output::PrintHelp();
          exit(0);
        case 'v':
          // Print version
          lili::output::PrintVersion();
          exit(0);
        default:
          // Throw error for unrecognized option
          std::cerr << "Unrecognized option: " << argv[i_arg] << std::endl;
          exit(1);
      }
    } else if (!has_input) {
      // Parse input file
      input.input_file() = argv[i_arg];
      input.Parse();
      has_input = true;
    } else {
      // Throw error for multiple input file
      std::cerr << "There are multiple input file" << std::endl;
      exit(1);
    }
  }

  // Check if there is input file
  if (!has_input) {
    // Throw error for no input file
    std::cerr << "No input file" << std::endl;
    // Print help
    lili::output::PrintHelp();
    exit(1);
  }

  return input;
}
}  // namespace lili::input