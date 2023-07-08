#pragma once
/**
 * @file mesh.hpp
 * @brief Header file for the Mesh class
 */

namespace lili::mesh {
template <typename T>
class Mesh {
 private:
  T* data_;  // Pointer to the data block

  bool is_data_init_;    // Whether the Mesh object has been initialized
  bool is_data_owner_;   // Whether the Mesh object owns the data block
  bool is_data_shared_;  // Whether the Mesh object shares the data block

  int n0_, n1_, n2_;     // Mesh size
  int n0g_, n1g_, n2g_;  // Number of ghost cells (assumed to be the same for
                         // all directions)

 public:
  // Empty constructor
  Mesh() : data_(nullptr), n0_(0), n1_(0), n2_(0), n0g_(0), n1g_(0), n2g_(0){};

  // Size-based initialization
  Mesh(int n0)
      : data_(nullptr),
        n0_(n0),
        n1_(1),
        n2_(1),
        n0g_(0),
        n1g_(0),
        n2g_(0){init_data()};

  Mesh(int n0, int n1)
      : data_(nullptr),
        n0_(n0),
        n1_(n1),
        n2_(1),
        n0g_(0),
        n1g_(0),
        n2g_(0){init_data()};

  Mesh(int n0, int n1, int n2)
      : data_(nullptr),
        n0_(n0),
        n1_(n1),
        n2_(n2),
        n0g_(0),
        n1g_(0),
        n2g_(0){init_data()};

  // Copy constructor
  // Compare Mesh sizes
  // Initialize data with empty array
  // Get subset of data
};
}  // namespace lili::mesh