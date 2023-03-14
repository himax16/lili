#pragma once
/**
 * @file mesh.hpp
 * @brief Header file for the Mesh class
 */

template <typename T>
class Mesh {
 private:
  T* data_;              // Pointer to the data block
  int n0_, n1_, n2_;     // Mesh size
  int n0g_, n1g_, n2g_;  // ???

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

  // Copy Mesh
  // Compare Mesh sizes
  // Initialize data with empty array
};