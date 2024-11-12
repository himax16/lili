#pragma once
/**
 * @file parameter.hpp
 * @brief Header file for storing global simulation parameters
 */

#include "output.hpp"

namespace lili {
extern int rank;   ///< MPI rank
extern int nproc;  ///< Number of MPI processes
extern std::string output_folder;  ///< Output folder
extern output::LiliCout lout;      ///< LiliCout object for output
extern output::LiliCerr lerr;      ///< LiliCerr object for error
}  // namespace lili
