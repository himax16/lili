/**
 * @file mesh.hpp
 * @brief Header only library for the Mesh related classes
 */
#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>

namespace lili::mesh {
/**
 * @brief Enumeration class for the ghost cell locations
 */
typedef enum {
  XPrev, /**< Previous X-axis ghost */
  XNext, /**< Next X-axis ghost */
  YPrev, /**< Previous Y-axis ghost */
  YNext, /**< Next Y-axis ghost */
  ZPrev, /**< Previous Z-axis ghost */
  ZNext  /**< Next Z-axis ghost */
} MeshGhostLocation;

/**
 * @brief Mesh size struct
 */
typedef struct {
  int dim;
  uint32_t nx, ny, nz;
  uint32_t ngx, ngy, ngz;
  double lx, ly, lz;
} MeshSize;

/**
 * @brief Mesh class
 */
template <typename T>
class Mesh {
 public:
  // Default constructor
  Mesh()
      : dim_(0),
        nx_(0),
        ny_(0),
        nz_(0),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        data_(nullptr){};

  // Size-based initialization
  Mesh(uint32_t nx)
      : dim_(1),
        nx_(nx),
        ny_(1),
        nz_(1),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        data_(nullptr) {
    InitializeData();
  };
  Mesh(uint32_t nx, uint32_t ny)
      : dim_(2),
        nx_(nx),
        ny_(ny),
        nz_(1),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        data_(nullptr) {
    InitializeData();
  };
  Mesh(uint32_t nx, uint32_t ny, uint32_t nz)
      : dim_(3),
        nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        data_(nullptr) {
    InitializeData();
  };
  Mesh(uint32_t nx, uint32_t ny, uint32_t nz, uint32_t ng)
      : dim_(3),
        nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(ng),
        ngy_(ng),
        ngz_(ng),
        data_(nullptr) {
    InitializeData();
  };
  Mesh(uint32_t nx, uint32_t ny, uint32_t nz, uint32_t ngx, uint32_t ngy,
       uint32_t ngz)
      : dim_(3),
        nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(ngx),
        ngy_(ngy),
        ngz_(ngz),
        data_(nullptr) {
    InitializeData();
  };
  Mesh(MeshSize domain_size)
      : dim_(domain_size.dim),
        nx_(domain_size.nx),
        ny_(domain_size.ny),
        nz_(domain_size.nz),
        ngx_(domain_size.ngx),
        ngy_(domain_size.ngy),
        ngz_(domain_size.ngz),
        data_(nullptr) {
    InitializeData();
  };

  // Copy constructor
  Mesh(const Mesh& other)
      : dim_(other.dim_),
        nx_(other.nx_),
        ny_(other.ny_),
        nz_(other.nz_),
        ngx_(other.ngx_),
        ngy_(other.ngy_),
        ngz_(other.ngz_),
        data_(nullptr) {
    InitializeData();
    std::copy(other.data_, other.data_ + other.nt(), data_);
  };

  // Move constructor
  Mesh(Mesh&& other) noexcept : Mesh() { swap(*this, other); };

  // Destructor
  ~Mesh() {
    if (data_ != nullptr) {
      delete[] data_;
    }
  };

  // Getters
  constexpr int dim() const { return dim_; };
  constexpr uint32_t nx() const { return nx_; };
  constexpr uint32_t ny() const { return ny_; };
  constexpr uint32_t nz() const { return nz_; };
  constexpr uint32_t ngx() const { return ngx_; };
  constexpr uint32_t ngy() const { return ngy_; };
  constexpr uint32_t ngz() const { return ngz_; };
  constexpr uint32_t ntx() const { return ntx_; };
  constexpr uint32_t nty() const { return nty_; };
  constexpr uint32_t ntz() const { return ntz_; };
  constexpr uint32_t nt() const { return nt_; };
  constexpr T* data() const { return data_; };

  // Swap data
  friend void swap(Mesh<T>& first, Mesh<T>& second) noexcept {
    using std::swap;
    swap(first.dim_, second.dim_);
    swap(first.nx_, second.nx_);
    swap(first.ny_, second.ny_);
    swap(first.nz_, second.nz_);
    swap(first.ngx_, second.ngx_);
    swap(first.ngy_, second.ngy_);
    swap(first.ngz_, second.ngz_);
    swap(first.ntx_, second.ntx_);
    swap(first.nty_, second.nty_);
    swap(first.ntz_, second.ntz_);
    swap(first.nt_, second.nt_);
    swap(first.data_, second.data_);
  }

  // Operators
  Mesh<T>& operator=(Mesh<T> other) {
    swap(*this, other);
    return *this;
  };

  // Raw access operator
  T operator()(uint32_t i) const { return data_[i]; };
  T& operator()(uint32_t i) { return data_[i]; };

  // Smart access operator
  T operator()(uint32_t i, uint32_t j, uint32_t k) const {
    return data_[ngx_ + i + ntx_ * (ngy_ + j + nty_ * (ngz_ + k))];
  };
  T& operator()(uint32_t i, uint32_t j, uint32_t k) {
    return data_[ngx_ + i + ntx_ * (ngy_ + j + nty_ * (ngz_ + k))];
  };

  // Update total mesh sizes
  void UpdateTotalSizes() {
    // Update dimension if needed
    if (dim_ == 0) {
      dim_ = (nz_ > 1) ? 3 : ((ny_ > 1) ? 2 : 1);
    }

    ntx_ = nx_ + 2 * ngx_;
    nty_ = ny_ + 2 * ngy_;
    ntz_ = nz_ + 2 * ngz_;

    nt_ = ntx_ * nty_ * ntz_;
  };

  // Compare Mesh sizes
  bool SameSizeAs(const Mesh& other) {
    return (nx_ == other.nx_ && ny_ == other.ny_ && nz_ == other.nz_ &&
            ngx_ == other.ngx_ && ngy_ == other.ngy_ && ngz_ == other.ngz_);
  };

  // Initialize data
  void InitializeData() {
    // Update total mesh sizes
    UpdateTotalSizes();

    // Allocate memory
    data_ = new T[nt_]();
  };

  // Ghost size utilities
  void CopyToGhost(const Mesh& other, MeshGhostLocation gl) {
    switch (gl) {
      case XPrev:
        // Make sure the other mesh has the same relevant size
        if (other.ny() != ny_ || other.nz() != nz_ || other.nx() < ngx_) {
          std::cerr << "Invalid ghost mesh size..." << std::endl;
          exit(2);
        } else {
          // Cache variable
          uint32_t noff = other.nx();
          // Copy data
          for (uint32_t i = -ngx_; i < 0; ++i) {
            for (uint32_t j = 0; j < ny_; ++j) {
              for (uint32_t k = 0; k < nz_; ++k) {
                (*this)(i, j, k) = other(noff + i, j, k);
              }
            }
          }
        }
        break;
      case XNext:
        // Make sure the other mesh has the same relevant size
        if (other.ny() != ny_ || other.nz() != nz_ || other.nx() < ngx_) {
          std::cerr << "Invalid ghost mesh size..." << std::endl;
          exit(2);
        } else {
          // Cache variable
          uint32_t noff = -nx_;
          // Copy data
          for (uint32_t i = nx_; i < (nx_ + ngx_); ++i) {
            for (uint32_t j = 0; j < ny_; ++j) {
              for (uint32_t k = 0; k < nz_; ++k) {
                (*this)(i, j, k) = other(noff + i, j, k);
              }
            }
          }
        }
        break;
      default:
        std::cerr << "Invalid ghost location..." << std::endl;
        break;
    }
  }

 private:
  int dim_;                   // Mesh dimension
  uint32_t nx_, ny_, nz_;     // Mesh sizes
  uint32_t ngx_, ngy_, ngz_;  // Ghost cells sizes (same for before and after)
  uint32_t ntx_, nty_, ntz_, nt_;  // Total mesh sizes (including ghost cells)

  T* data_;  // Pointer to the data block
};
}  // namespace lili::mesh