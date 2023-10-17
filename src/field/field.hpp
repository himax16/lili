/**
 * @file field.hpp
 * @brief Header only library for the field related routines
 */
#pragma once

#include "mesh.hpp"

namespace lili::mesh {
/**
 * @brief Field class
 */
class Field {
 public:
  // Constructor
  Field()
      : nx_(1),
        ny_(1),
        nz_(1),
        ngx_(0),
        ngy_(0),
        ngz_(0),
        dx_(1.0),
        dy_(1.0),
        dz_(1.0) {
    InitializeMesh();
  };

  // Size-based initialization
  Field(int nx, int ny, int nz, int ngx, int ngy, int ngz)
      : nx_(nx),
        ny_(ny),
        nz_(nz),
        ngx_(ngx),
        ngy_(ngy),
        ngz_(ngz),
        dx_(1.0),
        dy_(1.0),
        dz_(1.0) {
    InitializeMesh();
  };

  Field(const MeshSize& domain_size)
      : nx_(domain_size.nx),
        ny_(domain_size.ny),
        nz_(domain_size.nz),
        ngx_(domain_size.ngx),
        ngy_(domain_size.ngy),
        ngz_(domain_size.ngz),
        dx_(1.0),
        dy_(1.0),
        dz_(1.0) {
    InitializeMesh();
  };

  // Copy constructor
  Field(const Field& field)
      : nx_(field.nx_),
        ny_(field.ny_),
        nz_(field.nz_),
        ngx_(field.ngx_),
        ngy_(field.ngy_),
        ngz_(field.ngz_),
        dx_(field.dx_),
        dy_(field.dy_),
        dz_(field.dz_) {
    ex = field.ex;
    ey = field.ey;
    ez = field.ez;
    bx = field.bx;
    by = field.by;
    bz = field.bz;
    SyncSize();
  };

  // Move constructor
  Field(Field&& other) noexcept : Field() { swap(*this, other); };

  // Destructor
  ~Field() = default;

  // Swap function
  friend void swap(Field& first, Field& second) noexcept {
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

    swap(first.dx_, second.dx_);
    swap(first.dy_, second.dy_);
    swap(first.dz_, second.dz_);

    swap(first.ex, second.ex);
    swap(first.ey, second.ey);
    swap(first.ez, second.ez);
    swap(first.bx, second.bx);
    swap(first.by, second.by);
    swap(first.bz, second.bz);
  };

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
  constexpr double dx() const { return dx_; };
  constexpr double dy() const { return dy_; };
  constexpr double dz() const { return dz_; };

  void SyncSize() {
    // Sync sizes
    nx_ = bx.nx();
    ny_ = bx.ny();
    nz_ = bx.nz();
    ngx_ = bx.ngx();
    ngy_ = bx.ngy();
    ngz_ = bx.ngz();

    // Sync total mesh sizes
    dim_ = bx.dim();
    ntx_ = bx.ntx();
    nty_ = bx.nty();
    ntz_ = bx.ntz();
    nt_ = bx.nt();
  };

  void InitializeMesh() {
    // Initialize mesh
    ex = Mesh<double>(nx_, ny_, nz_, ngx_, ngy_, ngz_);
    ey = Mesh<double>(nx_, ny_, nz_, ngx_, ngy_, ngz_);
    ez = Mesh<double>(nx_, ny_, nz_, ngx_, ngy_, ngz_);
    bx = Mesh<double>(nx_, ny_, nz_, ngx_, ngy_, ngz_);
    by = Mesh<double>(nx_, ny_, nz_, ngx_, ngy_, ngz_);
    bz = Mesh<double>(nx_, ny_, nz_, ngx_, ngy_, ngz_);

    SyncSize();
  };

  // Public data members
  Mesh<double> ex, ey, ez;
  Mesh<double> bx, by, bz;

 private:
  int dim_;                   // Mesh dimension
  int nx_, ny_, nz_;          // Mesh sizes
  int ngx_, ngy_, ngz_;       // Ghost cells sizes (same for before and after)
  int ntx_, nty_, ntz_, nt_;  // Total mesh sizes (including ghost cells)

  double dx_, dy_, dz_;  // Mesh spacings
};

void LoadFieldTo(Field& field, const char* file_name,
                 bool include_ghost = false);

}  // namespace lili::mesh
