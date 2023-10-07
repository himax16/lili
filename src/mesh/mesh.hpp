/**
 * @file mesh.hpp
 * @brief Header only library for the Mesh related classes
 */
#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>

#ifndef __LILIM_DEFAULT_NGHOST
#define __LILIM_DEFAULT_NGHOST 2
#endif

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
  int nx, ny, nz;
  int ngx, ngy, ngz;
  double lx, ly, lz;
} MeshSize;

/**
 * @brief Mesh class
 * @tparam T Data type
 * @details
 * Mesh class with ghost cells and smart access operator. Data is stored in a
 * 1D array with column-major ordering.
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
  Mesh(int nx)
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
  Mesh(int nx, int ny)
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
  Mesh(int nx, int ny, int nz)
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
  Mesh(int nx, int ny, int nz, int ng)
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
  Mesh(int nx, int ny, int nz, int ngx, int ngy, int ngz)
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
  Mesh(const MeshSize& domain_size)
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

  // Swap function
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

  // Getters
  constexpr int dim() const { return dim_; };
  constexpr int nx() const { return nx_; };
  constexpr int ny() const { return ny_; };
  constexpr int nz() const { return nz_; };
  constexpr int ngx() const { return ngx_; };
  constexpr int ngy() const { return ngy_; };
  constexpr int ngz() const { return ngz_; };
  constexpr int ntx() const { return ntx_; };
  constexpr int nty() const { return nty_; };
  constexpr int ntz() const { return ntz_; };
  constexpr int nt() const { return nt_; };
  constexpr T* data() const { return data_; };

  // Operators
  Mesh<T>& operator=(Mesh<T> other) {
    swap(*this, other);
    return *this;
  };

  // Raw access operator
  T operator()(int i) const { return data_[i]; };
  T& operator()(int i) { return data_[i]; };

  // Smart access operator
  T operator()(int i, int j, int k) const {
    return data_[ngx_ + i + ntx_ * (ngy_ + j + nty_ * (ngz_ + k))];
  };
  T& operator()(int i, int j, int k) {
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

  /**
   * @brief Resize the mesh and clean up the data
   * @param nx New X-axis size
   * @param ny New Y-axis size
   * @param nz New Z-axis size
   * @param ngx New X-axis ghost size
   * @param ngy New Y-axis ghost size
   * @param ngz New Z-axis ghost size
   * @details
   * This function will resize the mesh and clean up the data. If the size is
   * changed, the data will be reallocated.
   */
  void Resize(int nx, int ny, int nz, int ngx, int ngy, int ngz) {
    // Update mesh sizes
    bool size_changed = false;

    if (nx != nx_) {
      nx_ = nx;
      size_changed = true;
    }
    if (ny != ny_) {
      ny_ = ny;
      size_changed = true;
    }
    if (nz != nz_) {
      nz_ = nz;
      size_changed = true;
    }
    if (ngx != ngx_) {
      ngx_ = ngx;
      size_changed = true;
    }
    if (ngy != ngy_) {
      ngy_ = ngy;
      size_changed = true;
    }
    if (ngz != ngz_) {
      ngz_ = ngz;
      size_changed = true;
    }

    // Update total mesh sizes
    UpdateTotalSizes();

    // Reallocate memory if needed
    if (size_changed) {
      if (data_ != nullptr) {
        delete[] data_;
      }
      data_ = new T[nt_]();
    }
  };

  /**
   * @brief Shrink the mesh inplace
   * @param nx New X-axis size
   * @param ny New Y-axis size
   * @param nz New Z-axis size
   * @param ngx New X-axis ghost size
   * @param ngy New Y-axis ghost size
   * @param ngz New Z-axis ghost size
   * @details
   * This function will shrink the mesh inplace. Crash if the new mesh size is
   * different from the current mesh size.
   */
  void Shrink(int nx, int ny, int nz, int ngx, int ngy, int ngz) {
    // Store the old size
    int old_nt = nt_;

    // Update mesh sizes
    nx_ = nx;
    ny_ = ny;
    nz_ = nz;
    ngx_ = ngx;
    ngy_ = ngy;
    ngz_ = ngz;

    // Update total mesh sizes
    UpdateTotalSizes();

    // Check if the new mesh size is different from the old one
    if (nt_ != old_nt) {
      std::cerr << "Cannot shrink the mesh inplace..." << std::endl;
      exit(2);
    }
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
          int noff = other.nx();
          // Copy data
          for (int i = -ngx_; i < 0; ++i) {
            for (int j = 0; j < ny_; ++j) {
              for (int k = 0; k < nz_; ++k) {
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
          int noff = -nx_;
          // Copy data
          for (int i = nx_; i < (nx_ + ngx_); ++i) {
            for (int j = 0; j < ny_; ++j) {
              for (int k = 0; k < nz_; ++k) {
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
  };

 private:
  int dim_;                   // Mesh dimension
  int nx_, ny_, nz_;          // Mesh sizes
  int ngx_, ngy_, ngz_;       // Ghost cells sizes (same for before and after)
  int ntx_, nty_, ntz_, nt_;  // Total mesh sizes (including ghost cells)

  T* data_;  // Pointer to the data block
};

void SaveMesh(Mesh<double>& mesh, const char* file_name, const char* data_name,
              bool include_ghost = false);
void LoadMeshTo(Mesh<double>& mesh, const char* file_name,
                const char* data_name, bool include_ghost = false);
}  // namespace lili::mesh