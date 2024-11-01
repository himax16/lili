/**
 * @file    input.cpp
 * @brief   Source file for processing CLI and user inputs
 */
#include "input.hpp"

#include <string>

#include "json.hpp"
#include "parameter.hpp"

// Simplify namespace
using json = nlohmann::json;

namespace lili::input {
std::string InputTypeToString(InputType input_type) {
  switch (input_type) {
    case InputType::None:
      return "None";
    case InputType::Initial:
      return "Initial";
    case InputType::Restart:
      return "Restart";
    case InputType::TestParticle:
      return "TestParticle";
    default:
      return "None";
  }
}

void InputParticles::Print() {
  lili::lout << "==============================" << std::endl;
  lili::lout << "Name          : " << name << std::endl;
  lili::lout << "n             : " << n << std::endl;
  lili::lout << "q             : " << q << std::endl;
  lili::lout << "m             : " << m << std::endl;
  lili::lout << "Pos. dist.    : ";
  switch (pos_dist) {
    case PPosDist::Stationary:
      lili::lout << "Stationary" << std::endl;
      break;
    case PPosDist::Uniform:
      lili::lout << "Uniform" << std::endl;
      break;
    default:
      lili::lout << "Unknown" << std::endl;
      break;
  }
  lili::lout << "  param       : ";
  for (auto& p : pos_dist_param) {
    lili::lout << p << " ";
  }
  lili::lout << std::endl;
  lili::lout << "Vel. dist.    : ";
  switch (vel_dist) {
    case PVelDist::Maxwellian:
      lili::lout << "Maxwellian" << std::endl;
      break;
    default:
      lili::lout << "Unknown" << std::endl;
      break;
  }
  lili::lout << "  param       : ";
  for (auto& p : vel_dist_param) {
    lili::lout << p << " ";
  }
  lili::lout << std::endl;
  lili::lout << "  offset      : ";
  for (auto& p : vel_offset) {
    lili::lout << p << " ";
  }
  lili::lout << std::endl;
  lili::lout << "==============================" << std::endl;
}

Input::Input() {
  input_file_ = "";
  problem_name_ = "LILI";
  input_type_ = InputType::None;
}

Input::Input(const char* in_file) {
  input_file_ = in_file;
  problem_name_ = "LILI";
  input_type_ = InputType::None;
}

Input::Input(const Input& input) {
  input_file_ = input.input_file_;
  problem_name_ = input.problem_name_;
  restart_file_ = input.restart_file_;

  input_type_ = input.input_type_;

  mesh_ = input.mesh_;
  particles_ = input.particles_;
  integrator_ = input.integrator_;
}

void swap(Input& first, Input& second) {
  using std::swap;

  swap(first.input_file_, second.input_file_);
  swap(first.problem_name_, second.problem_name_);
  swap(first.restart_file_, second.restart_file_);

  swap(first.input_type_, second.input_type_);

  swap(first.mesh_, second.mesh_);
  swap(first.particles_, second.particles_);
  swap(first.integrator_, second.integrator_);
}

void Input::Parse() {
  // Open input file
  std::ifstream ifs(input_file_.c_str());
  if (!ifs.is_open()) {
    std::cerr << "Cannot open input file: " << input_file_ << std::endl;
    lili::output::LiliExit(2);
  }

  // Parse the JSON file ignoring comment
  json j;
  try {
    j = json::parse(ifs,
                    /* callback */ nullptr,
                    /* allow exceptions */ true,
                    /* ignore comments */ true);
  } catch (json::parse_error& e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
    lili::output::LiliExit(2);
  }

  // Close input file
  ifs.close();

  // Check the type of input file
  if (!j.contains("input_type")) {
    std::cerr << "No input type in " << input_file_ << std::endl;
    lili::output::LiliExit(2);
  } else {
    std::string input_type_str = j.at("input_type").get<std::string>();
    if (strcmp(input_type_str.c_str(), "initial") == 0) {
      input_type_ = InputType::Initial;
    } else if (strcmp(input_type_str.c_str(), "restart") == 0) {
      input_type_ = InputType::Restart;
    } else if (strcmp(input_type_str.c_str(), "test_particle") == 0) {
      input_type_ = InputType::TestParticle;
    } else {
      std::cerr << "Unrecognized input type in " << input_type_str << std::endl;
      std::cerr << "Available input type: [initial | restart]" << std::endl;
      lili::output::LiliExit(2);
    }
  }

  // Parse problem name
  if (!j.contains("problem_name")) {
    std::cerr << "No problem name in " << input_file_ << std::endl;
    std::cerr << "Using default problem name: LILI" << std::endl;
  } else {
    problem_name_ = j.at("problem_name").get<std::string>();
  }

  // Parse restart file
  if (input_type_ == InputType::Restart ||
      input_type_ == InputType::TestParticle) {
    if (!j.contains("restart_file")) {
      std::cerr << "No restart file in " << input_file_ << std::endl;
      lili::output::LiliExit(2);
    } else {
      restart_file_ = j.at("restart_file").get<std::string>();
    }
  }

  // Parse mesh
  if (j.contains("mesh")) {
    auto j_mesh = j.at("mesh");

    // Parse mesh dimension
    mesh_.dim = j_mesh.at("dimension").get<int>();
    if (mesh_.dim < 1 || mesh_.dim > 3) {
      std::cerr << "Invalid mesh dimension in " << input_file_ << std::endl;
      lili::output::LiliExit(2);
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

    // Calculate the starting point of the mesh
    mesh_.x0 = 0.;
    mesh_.y0 = 0.;
    mesh_.z0 = 0.;
  }

  // Parse particles
  if (j.contains("particles")) {
    // Iterate over all species
    for (auto& [key, val] : j.at("particles").items()) {
      InputParticles species;
      species.name = key;

      // Parse particle variables
      species.n = val.at("n").get<int>();
      species.q = val.at("q").get<double>();
      species.m = val.at("m").get<double>();

      // Parse tracking variables
      if (val.contains("track")) {
        species.n_track = val.value("n_track", 0);
        species.dl_track = val.value("dl_track", 1);
        species.dtrack_save = val.value("dtrack_save", 0);
      } else {
        species.n_track = 0;
        species.dl_track = 1;
        species.dtrack_save = 0;
      }

      // Parse particle position distribution
      if (val.contains("position_distribution")) {
        auto& pdist = val.at("position_distribution");
        std::string pdist_str = pdist.value("type", "stationary");
        if (strcmp(pdist_str.c_str(), "stationary") == 0) {
          species.pos_dist = PPosDist::Stationary;
        } else if (strcmp(pdist_str.c_str(), "uniform") == 0) {
          species.pos_dist = PPosDist::Uniform;

          // Set position distribution parameters
          if (pdist.contains("param")) {
            // Read the value as a vector
            species.pos_dist_param =
                pdist.at("param").get<std::vector<double>>();
          } else {
            // Set default param to the mesh positions
            species.pos_dist_param.push_back(mesh_.x0);
            species.pos_dist_param.push_back(mesh_.x0 + mesh_.lx);
            species.pos_dist_param.push_back(mesh_.y0);
            species.pos_dist_param.push_back(mesh_.y0 + mesh_.ly);
            species.pos_dist_param.push_back(mesh_.z0);
            species.pos_dist_param.push_back(mesh_.z0 + mesh_.lz);
          }
        } else {
          std::cerr << "Unrecognized position distribution: " << pdist_str
                    << std::endl;
          std::cerr << "Available position distribution: [stationary | uniform]"
                    << std::endl;
          lili::output::LiliExit(2);
        }
      }

      // Parse particle velocity distribution
      if (val.contains("velocity_distribution")) {
        auto& vdist = val.at("velocity_distribution");
        std::string vdist_str = vdist.value("type", "uniform");
        if (strcmp(vdist_str.c_str(), "maxwellian") == 0) {
          species.vel_dist = PVelDist::Maxwellian;
        } else {
          std::cerr << "Unrecognized velocity distribution: " << vdist_str
                    << std::endl;
          std::cerr << "Available velocity distribution: [ maxwellian ]"
                    << std::endl;
          lili::output::LiliExit(2);
        }

        // Set velocity distribution parameters
        if (vdist.contains("param")) {
          // Read the value as a vector
          species.vel_dist_param = vdist.at("param").get<std::vector<double>>();
        } else {
          std::cerr << "No velocity distribution parameters for " << key
                    << std::endl;
          lili::output::LiliExit(2);
        }

        // Set velocity offset
        if (vdist.contains("offset")) {
          // Read the value as a vector
          species.vel_offset = vdist.at("offset").get<std::vector<double>>();
        }
      }

      // Add species to the list
      particles_.push_back(species);
    }
  }

  // Parse integrator
  if (j.contains("integrator")) {
    // Parse time step
    integrator_.dt = j.at("integrator").value("dt", 1.0);

    // Parse number of time steps
    integrator_.n_loop = j.at("integrator").value("n_loop", 1);
  }
}

Input ParseArguments(int argc, char** argv, lili::output::LiliCout& lout) {
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
            lili::output::PrintHelp(lout);
            lili::output::LiliExit(0);
          } else if (strcmp(argv[i_arg], "--version") == 0) {
            // Print version
            lili::output::PrintVersion(lout);
            lili::output::LiliExit(0);
          } else if (strcmp(argv[i_arg], "--input") == 0) {
            if (has_input) {
              // Throw error for multiple input file
              std::cerr << "There are multiple input file" << std::endl;
              lili::output::LiliExit(1);
            }
            // Parse input file
            input.input_file() = argv[++i_arg];
            input.Parse();
            has_input = true;
          } else if (strcmp(argv[i_arg], "--") == 0) {
            // End of options
            ++i_arg;
          } else {
            // Throw error for unrecognized option
            std::cerr << "Unrecognized option: " << argv[i_arg] << std::endl;
            lili::output::LiliExit(1);
          }
          break;
        case 'i':
          if (has_input) {
            // Throw error for multiple input file
            std::cerr << "There are multiple input file" << std::endl;
            lili::output::LiliExit(1);
          }
          // Parse input file
          input.input_file() = argv[++i_arg];
          input.Parse();
          has_input = true;
          break;
        case 'h':
          // Print help
          lili::output::PrintHelp(lout);
          lili::output::LiliExit(0);
          break;
        case 'v':
          // Print version
          lili::output::PrintVersion(lout);
          lili::output::LiliExit(0);
          break;
        default:
          // Throw error for unrecognized option
          std::cerr << "Unrecognized option: " << argv[i_arg] << std::endl;
          lili::output::LiliExit(1);
      }
    } else if (!has_input) {
      // Parse input file
      input.input_file() = argv[i_arg];
      input.Parse();
      has_input = true;
    } else {
      // Throw error for multiple input file
      std::cerr << "There are multiple input file" << std::endl;
      lili::output::LiliExit(1);
    }
  }

  // Check if there is input file
  if (!has_input) {
    // Throw error for no input file
    std::cerr << "No input file" << std::endl;
    // Print help
    lili::output::PrintHelp();
    lili::output::LiliExit(1);
  }

  return input;
}
}  // namespace lili::input
