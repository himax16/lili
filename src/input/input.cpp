/*
 * @file    input.cpp
 * @brief   Source file for processing CLI and user inputs
 */
#include "input.hpp"

#include "json.hpp"
#include "output.hpp"

// Simplify namespace
using json = nlohmann::json;

std::string lili::input::input_file;
std::string lili::input::problem_name;
int lili::input::input_type;

/**
 * @brief Parse input file
 *
 * @param[in] input_file
 * Input file
 */
void lili::input::parse_input(char *in_file) {
  // Open input file
  std::ifstream ifs(in_file);
  if (!ifs.is_open()) {
    std::cerr << "Cannot open input file: " << in_file << std::endl;
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
  input_file = in_file;

  // Close input file
  ifs.close();

  // Check the type of input file
  if (!j.contains("input_type")) {
    std::cerr << "No input type in " << in_file << std::endl;
    exit(2);
  } else {
    std::string input_type_str = j.at("input_type").get<std::string>();
    if (strcmp(input_type_str.c_str(), "initial") == 0) {
      input_type = 0;
    } else if (strcmp(input_type_str.c_str(), "restart") == 0) {
      input_type = 1;
    } else {
      std::cerr << "Unrecognized input type in " << input_type_str << std::endl;
      std::cerr << "Available input type: [initial | restart]" << std::endl;
      exit(2);
    }
  }

  // Parse problem name
  if (!j.contains("problem_name")) {
    std::cerr << "No problem name in " << in_file << std::endl;
    std::cerr << "Using default problem name: LILI" << std::endl;
  } else {
    problem_name = j.at("problem_name").get<std::string>();
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
void lili::input::parse_arguments(int argc, char *argv[]) {
  // Variable declaration
  int i_arg = 0;
  bool has_input = false;

  // Parse command line arguments
  for (i_arg = 1; i_arg < argc; ++i_arg) {
    if (argv[i_arg][0] == '-') {
      switch (argv[i_arg][1]) {
        case '-':
          // Long option
          if (strcmp(argv[i_arg], "--help") == 0) {
            // Print help
            lili::output::print_help();
            exit(0);
          } else if (strcmp(argv[i_arg], "--version") == 0) {
            // Print version
            lili::output::print_version();
            exit(0);
          } else if (strcmp(argv[i_arg], "--input") == 0) {
            if (has_input) {
              // Throw error for multiple input file
              std::cerr << "There are multiple input file" << std::endl;
              exit(1);
            }
            // Parse input file
            parse_input(argv[++i_arg]);
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
          parse_input(argv[++i_arg]);
          has_input = true;
        case 'h':
          // Print help
          lili::output::print_help();
          exit(0);
        case 'v':
          // Print version
          lili::output::print_version();
          exit(0);
        default:
          // Throw error for unrecognized option
          std::cerr << "Unrecognized option: " << argv[i_arg] << std::endl;
          exit(1);
      }
    } else if (!has_input) {
      // Parse input file
      parse_input(argv[i_arg]);
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
    exit(1);
  }
}