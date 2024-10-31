/**
 * @file mesh.hpp
 * @brief Header file for the Mesh related classes
 */
#pragma once

#include <algorithm>
#include <cstdint>

#include "output.hpp"

#ifndef __LILIM_DEFAULT_NGHOST
/**
 * @brief Default number of ghost cells
 */
#define __LILIM_DEFAULT_NGHOST 2
#endif

/**
 * @brief
 * Namespace for LILI mesh related routines
 */
namespace lili::mesh {
/**
 * @brief Enumeration class for the ghost cell locations
 */
enum class MeshGhostLocation : uint8_t {
  XPrev = 0,  ///< Previous X-axis ghost
  XNext = 1,  ///< Next X-axis ghost
  YPrev = 2,  ///< Previous Y-axis ghost
  YNext = 3,  ///< Next Y-axis ghost
  ZPrev = 4,  ///< Previous Z-axis ghost
  ZNext = 5,  ///< Next Z-axis ghost
};

/**
 * @brief Struct to store Mesh size information
 */
struct MeshSize {
  int dim;    ///< Dimension of the mesh
  int nx;     ///< Number of cells in the X-axis
  int ny;     ///< Number of cells in the Y-axis
  int nz;     ///< Number of cells in the Z-axis
  int ngx;    ///< Number of ghost cells in the X-axis
  int ngy;    ///< Number of ghost cells in the Y-axis
  int ngz;    ///< Number of ghost cells in the Z-axis
  double lx;  ///< Length of the mesh in the X-axis
  double ly;  ///< Length of the mesh in the Y-axis
  double lz;  ///< Length of the mesh in the Z-axis
  double x0;  ///< Starting point of the mesh in the X-axis
  double y0;  ///< Starting point of the mesh in the Y-axis
  double z0;  ///< Starting point of the mesh in the Z-axis
};

/**
 * @brief Simple class to store Mesh size information
 *
 * @details
 * This class is used to store the Mesh size information. All of the members are
 * public.
 */
class MeshSizeC {
 public:
  int dim;    ///< Dimension of the mesh
  int nx;     ///< Number of cells in the X-axis
  int ny;     ///< Number of cells in the Y-axis
  int nz;     ///< Number of cells in the Z-axis
  int ngx;    ///< Number of ghost cells in the X-axis
  int ngy;    ///< Number of ghost cells in the Y-axis
  int ngz;    ///< Number of ghost cells in the Z-axis
  double lx;  ///< Length of the mesh in the X-axis
  double ly;  ///< Length of the mesh in the Y-axis
  double lz;  ///< Length of the mesh in the Z-axis
  double x0;  ///< Starting point of the mesh in the X-axis
  double y0;  ///< Starting point of the mesh in the Y-axis
  double z0;  ///< Starting point of the mesh in the Z-axis
};

/**
 * @brief Function to print MeshSize information
 *
 * @param mesh_size Mesh size information
 * @details
 * Print the MeshSize information to the standard output.
 *
 * Example:
 * ```cpp
 * ====== Mesh information ======
 * dim = 3
 * n   = (100, 100, 100)
 * ng  = (2, 2, 2)
 * l   = (1.0, 1.0, 1.0)
 * r0  = (0.0, 0.0, 0.0)
 * ==============================
 * ```
 */
void PrintMeshSize(const MeshSize& mesh_size, lili::output::LiliCout& lout);

/**
 * @brief Function to recalculate the dimension of the MeshSize
 *
 * @param mesh_size Mesh size information
 * @details
 * Recalculate the dimension of the MeshSize based on the number of cells in
 * each axis.
 * \f[
 * \text{dim} = \begin{cases}
 * 3 & \text{if } n_z > 1 \\
 * 2 & \text{if } n_y > 1 \\
 * 1 & \text{otherwise}
 * \end{cases}
 * \f]
 */
void UpdateMeshSizeDim(MeshSize& mesh_size);

/**
 * @brief Mesh class
 *
 * @tparam T Data type
 * @details
 * Base mesh class with ghost cells and smart access operator.
 * Data is stored in a 1D array with column-major ordering.
 */
template <typename T>
class Mesh {
 public:
  /**
   * @brief Default constructor for the Mesh class
   * @details
   * This constructor will initialize the Mesh class with zero sizes.
   * The data will not be initialized.
   */
  Mesh()
      : dim_(0),
        nx_(0),
        ny_(0),
        nz_(0),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        data_(nullptr) {}

  // Size-based initialization
  /**
   * @brief 1D Mesh constructor
   *
   * @param nx Number of cells in the X-axis \f$n_x\f$.
   * @details
   * This constructor will initialize 1D Mesh class with zero ghost cells.
   * Initialize the data with the given size.
   */
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
  }

  /**
   * @brief 2D Mesh constructor
   *
   * @param nx Number of cells in the X-axis \f$n_x\f$.
   * @param ny Number of cells in the Y-axis \f$n_y\f$.
   * @details
   * This constructor will initialize 2D Mesh class with zero ghost cells.
   * Initialize the data with the given size.
   */
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
  }

  /**
   * @brief 3D Mesh constructor
   *
   * @param nx Number of cells in the X-axis \f$n_x\f$.
   * @param ny Number of cells in the Y-axis \f$n_y\f$.
   * @param nz Number of cells in the Z-axis \f$n_z\f$.
   * @details
   * This constructor will initialize 3D Mesh class with zero ghost cells.
   * Initialize the data with the given size.
   */
  Mesh(int nx, int ny, int nz)
      : dim_(nz > 1 ? 3 : (ny > 1 ? 2 : 1)),
        nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        data_(nullptr) {
    InitializeData();
  }

  /**
   * @brief 3D Mesh constructor with ghost cells
   *
   * @param nx Number of cells in the X-axis \f$n_x\f$.
   * @param ny Number of cells in the Y-axis \f$n_y\f$.
   * @param nz Number of cells in the Z-axis \f$n_z\f$.
   * @param ng Number of ghost cells \f$n_g\f$ in all axis.
   * @details
   * This constructor will initialize 3D Mesh class with the same number of
   * ghost cells in all axis.
   * Initialize the data with the given size.
   */
  Mesh(int nx, int ny, int nz, int ng)
      : dim_(nz > 1 ? 3 : (ny > 1 ? 2 : 1)),
        nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(ng),
        ngy_(ng),
        ngz_(ng),
        data_(nullptr) {
    InitializeData();
  }

  /**
   * @brief 3D Mesh constructor with custom ghost cells
   *
   * @param nx Number of cells in the X-axis \f$n_x\f$.
   * @param ny Number of cells in the Y-axis \f$n_y\f$.
   * @param nz Number of cells in the Z-axis \f$n_z\f$.
   * @param ngx Number of ghost cells \f$n_{gx}\f$ in the X-axis.
   * @param ngy Number of ghost cells \f$n_{gy}\f$ in the Y-axis.
   * @param ngz Number of ghost cells \f$n_{gz}\f$ in the Z-axis.
   * @details
   * This constructor will initialize 3D Mesh class with custom number of ghost
   * cells in each axis.
   * Initialize the data with the given size.
   */
  Mesh(int nx, int ny, int nz, int ngx, int ngy, int ngz)
      : dim_(nz > 1 ? 3 : (ny > 1 ? 2 : 1)),
        nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(ngx),
        ngy_(ngy),
        ngz_(ngz),
        data_(nullptr) {
    InitializeData();
  }

  /**
   * @brief Mesh constructor using MeshSize struct
   *
   * @param domain_size MeshSize struct containing the mesh size information
   * @details
   * This constructor will initialize the Mesh class with the given MeshSize
   * struct.
   * Initialize the data with the given size.
   */
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
  }

  /**
   * @brief Copy constructor for the Mesh class
   *
   * @param other Other Mesh object
   * @details
   * This constructor will copy the data from the other object of Mesh class.
   */
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
  }

  /**
   * @brief Move constructor for the Mesh class
   *
   * @param other Pointer to the other Mesh object
   * @details
   * This constructor will move the data from the other object of Mesh class.
   * The other object will be reset to the default state.
   */
  Mesh(Mesh&& other) noexcept : Mesh() { swap(*this, other); }

  /**
   * @brief Destructor for the Mesh class
   */
  ~Mesh() {
    if (data_ != nullptr) {
      delete[] data_;
    }
  }

  /**
   * @brief Function to swap the data between two Mesh objects
   *
   * @param first First Mesh object
   * @param second Second Mesh object
   * @details
   * This function will swap the data between two Mesh objects in-place using
   * std::swap.
   */
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
  /// @cond GETTERS
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
  /// @endcond

  // Operators
  /// @cond OPERATORS
  /**
   * @brief Assignment operator for the Mesh class object for another Mesh
   * class object
   *
   * @param other Other Mesh class object
   * @return Mesh<T>& Reference to the current Mesh object
   */
  Mesh<T>& operator=(Mesh<T> other) {
    swap(*this, other);
    return *this;
  };

  /**
   * @brief Assignment operator for the Mesh class object for a scalar value
   *
   * @param value Scalar value
   * @return Mesh<T>& Reference to the current Mesh object
   */
  Mesh<T>& operator=(T value) {
    for (int i = 0; i < nt_; ++i) {
      data_[i] = value;
    }
    return *this;
  };

  /**
   * @brief Addition assignment operator for the Mesh class object for a scalar
   * value
   *
   * @param value Scalar value
   * @return Mesh<T>& Reference to the current Mesh object
   */
  Mesh<T>& operator+=(T value) {
    for (int i = 0; i < nt_; ++i) {
      data_[i] += value;
    }
    return *this;
  };

  // Raw access operator (1D)
  T operator()(int i) const { return data_[i]; };
  T& operator()(int i) { return data_[i]; };

  // Smart access operator (3D)
  T operator()(int i, int j, int k) const {
    return data_[ngx_ + i + ntx_ * (ngy_ + j + nty_ * (ngz_ + k))];
  };
  T& operator()(int i, int j, int k) {
    return data_[ngx_ + i + ntx_ * (ngy_ + j + nty_ * (ngz_ + k))];
  };
  /// @endcond

  /**
   * @brief Recalculate the total mesh sizes based on the current sizes
   */
  void UpdateTotalSizes() {
    dim_ = (nz_ > 1) ? 3 : ((ny_ > 1) ? 2 : 1);

    ntx_ = nx_ + 2 * ngx_;
    nty_ = ny_ + 2 * ngy_;
    ntz_ = nz_ + 2 * ngz_;

    nt_ = ntx_ * nty_ * ntz_;
  };

  /**
   * @brief Comparator to check if the mesh size is the same as the other mesh
   *
   * @param other Other Mesh object
   */
  bool SameSizeAs(const Mesh& other) {
    return (nx_ == other.nx_ && ny_ == other.ny_ && nz_ == other.nz_ &&
            ngx_ == other.ngx_ && ngy_ == other.ngy_ && ngz_ == other.ngz_);
  };

  /**
   * @brief Initialize the data block for the mesh
   */
  void InitializeData() {
    // Update total mesh sizes
    UpdateTotalSizes();

    // Check if the data is already allocated
    if (data_ != nullptr) {
      delete[] data_;
    }

    // Allocate memory
    data_ = new T[nt_]();
  };

  /**
   * @brief Resize the mesh and clean up the data
   *
   * @param nx New X-axis size \f$n_x\f$
   * @param ny New Y-axis size \f$n_y\f$
   * @param nz New Z-axis size \f$n_z\f$
   * @param ngx New X-axis ghost size \f$n_{gx}\f$
   * @param ngy New Y-axis ghost size \f$n_{gy}\f$
   * @param ngz New Z-axis ghost size \f$n_{gz}\f$
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
   *
   * @param nx New X-axis size \f$n_x\f$
   * @param ny New Y-axis size \f$n_y\f$
   * @param nz New Z-axis size \f$n_z\f$
   * @param ngx New X-axis ghost size \f$n_{gx}\f$
   * @param ngy New Y-axis ghost size \f$n_{gy}\f$
   * @param ngz New Z-axis ghost size \f$n_{gz}\f$
   * @details
   * This function will shrink the mesh inplace.
   * Crash if the new mesh size is different from the current mesh size.
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

  /**
   * @brief Copy data from other Mesh to the current Mesh ghost cells
   *
   * @param other Other mesh
   * @param gl Ghost location
   */
  void CopyToGhost(const Mesh& other, MeshGhostLocation gl) {
    switch (gl) {
      case MeshGhostLocation::XPrev:
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
      case MeshGhostLocation::XNext:
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
      case MeshGhostLocation::YPrev:
        // Make sure the other mesh has the same relevant size
        if (other.nx() != nx_ || other.nz() != nz_ || other.ny() < ngy_) {
          std::cerr << "Invalid ghost mesh size..." << std::endl;
          exit(2);
        } else {
          // Cache variable
          int noff = other.ny();
          // Copy data
          for (int i = 0; i < nx_; ++i) {
            for (int j = -ngy_; j < 0; ++j) {
              for (int k = 0; k < nz_; ++k) {
                (*this)(i, j, k) = other(i, noff + j, k);
              }
            }
          }
        }
        break;
      case MeshGhostLocation::YNext:
        // Make sure the other mesh has the same relevant size
        if (other.nx() != nx_ || other.nz() != nz_ || other.ny() < ngy_) {
          std::cerr << "Invalid ghost mesh size..." << std::endl;
          exit(2);
        } else {
          // Cache variable
          int noff = -ny_;
          // Copy data
          for (int i = 0; i < nx_; ++i) {
            for (int j = ny_; j < (ny_ + ngy_); ++j) {
              for (int k = 0; k < nz_; ++k) {
                (*this)(i, j, k) = other(i, noff + j, k);
              }
            }
          }
        }
        break;
      case MeshGhostLocation::ZPrev:
        // Make sure the other mesh has the same relevant size
        if (other.nx() != nx_ || other.ny() != ny_ || other.nz() < ngz_) {
          std::cerr << "Invalid ghost mesh size..." << std::endl;
          exit(2);
        } else {
          // Cache variable
          int noff = other.nz();
          // Copy data
          for (int i = 0; i < nx_; ++i) {
            for (int j = 0; j < ny_; ++j) {
              for (int k = -ngz_; k < 0; ++k) {
                (*this)(i, j, k) = other(i, j, noff + k);
              }
            }
          }
        }
        break;
      case MeshGhostLocation::ZNext:
        // Make sure the other mesh has the same relevant size
        if (other.nx() != nx_ || other.ny() != ny_ || other.nz() < ngz_) {
          std::cerr << "Invalid ghost mesh size..." << std::endl;
          exit(2);
        } else {
          // Cache variable
          int noff = -nz_;
          // Copy data
          for (int i = 0; i < nx_; ++i) {
            for (int j = 0; j < ny_; ++j) {
              for (int k = nz_; k < (nz_ + ngz_); ++k) {
                (*this)(i, j, k) = other(i, j, noff + k);
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

  /**
   * @brief Linear interpolation
   *
   * @param x Data point location relative to the mesh \f$x^\prime\f$
   * @return Interpolated value at \f$x^\prime\f$
   */
  T LinearInterpolation(double x) const {
    // Cache variables
    int ix = static_cast<int>(x);
    double xd = x - ix;

    // Interpolate
    return (1.0 - xd) * (*this)(ix, 0, 0) + xd * (*this)(ix + 1, 0, 0);
  };

  /**
   * @brief Bilenar interpolation
   *
   * @param x Data point location relative to the mesh \f$x^\prime\f$
   * @param y Data point location relative to the mesh \f$y^\prime\f$
   * @return Interpolated value at \f$(x^\prime, y^\prime)\f$
   */
  T BilinearInterpolation(double x, double y) const {
    // Cache variables
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    double xd = x - ix;
    double yd = y - iy;

    // Interpolate
    return (1.0 - xd) *
               ((1.0 - yd) * (*this)(ix, iy, 0) + yd * (*this)(ix, iy + 1, 0)) +
           xd * ((1.0 - yd) * (*this)(ix + 1, iy, 0) +
                 yd * (*this)(ix + 1, iy + 1, 0));
  }

  /**
   * @brief Trilinear interpolation
   *
   * @param x Data point location relative to the mesh \f$x^\prime\f$
   * @param y Data point location relative to the mesh \f$y^\prime\f$
   * @param z Data point location relative to the mesh \f$z^\prime\f$
   * @return Interpolated value at \f$(x^\prime, y^\prime, z^\prime)\f$
   */
  T TrilinearInterpolation(double x, double y, double z) const {
    // Cache variables
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);
    int iz = static_cast<int>(z);

    double xd = x - ix;
    double yd = y - iy;
    double zd = z - iz;

    // Interpolate
    return (1.0 - xd) * ((1.0 - yd) * ((1.0 - zd) * (*this)(ix, iy, iz) +
                                       zd * (*this)(ix, iy, iz + 1)) +
                         yd * ((1.0 - zd) * (*this)(ix, iy + 1, iz) +
                               zd * (*this)(ix, iy + 1, iz + 1))) +
           xd * ((1.0 - yd) * ((1.0 - zd) * (*this)(ix + 1, iy, iz) +
                               zd * (*this)(ix + 1, iy, iz + 1)) +
                 yd * ((1.0 - zd) * (*this)(ix + 1, iy + 1, iz) +
                       zd * (*this)(ix + 1, iy + 1, iz + 1)));
  };

  /**
   * @brief Interpolation function
   *
   * @param x Data point location relative to the mesh \f$x^\prime\f$
   * @param y Data point location relative to the mesh \f$y^\prime\f$
   * @param z Data point location relative to the mesh \f$z^\prime\f$
   * @return Interpolated value at \f$(x^\prime, y^\prime, z^\prime)\f$
   * @details
   * This function will automatically choose the interpolation method based on
   * the mesh dimension. Ignore the extra arguments if the mesh is in lower
   * dimension.
   */
  T Interpolation(double x, double y, double z) const {
    if (dim_ == 2) {
      return BilinearInterpolation(x, y);
    } else if (dim_ == 3) {
      return TrilinearInterpolation(x, y, z);
    } else {
      return LinearInterpolation(x);
    }
  }

 private:
  int dim_;                   // Mesh dimension
  int nx_, ny_, nz_;          // Mesh sizes
  int ngx_, ngy_, ngz_;       // Ghost cells sizes (same for before and after)
  int ntx_, nty_, ntz_, nt_;  // Total mesh sizes (including ghost cells)

  T* data_;  // Pointer to the data block
};

/**
 * @brief Function to save Mesh data to a single file.
 *
 * @warning This function is not efficient for multiple calls. Use
 * lili::output::MeshStream instead.
 *
 * @param[in] mesh Mesh data
 * @param[in] file_name HDF5 file name
 * @param[in] data_name HDF5 dataset name
 * @param[in] include_ghost Save ghost cells in the data
 * @details
 * This function will save the Mesh data to a single HDF5 file. The data will be
 * saved in a dataset with the given name. If the file exists, the dataset will
 * be overwritten.
 */
void SaveMesh(Mesh<double>& mesh, const char* file_name, const char* data_name,
              bool include_ghost = false);

/**
 * @brief Function to load Mesh data from a file
 *
 * @param mesh Mesh data
 * @param file_name HDF5 file name
 * @param data_name HDF5 dataset name
 * @param include_ghost Load ghost cells in the data
 */
void LoadMeshTo(Mesh<double>& mesh, const char* file_name,
                const char* data_name, bool include_ghost = false);
}  // namespace lili::mesh
