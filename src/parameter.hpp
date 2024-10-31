#pragma once
/**
 * @file parameter.hpp
 * @brief Header file for storing global simulation parameters
 */

namespace lili {
extern int rank, nproc;  ///< MPI rank and size
extern const int nl_time;  ///< Number of loops between time printout
}  // namespace lili
