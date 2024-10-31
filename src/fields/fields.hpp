/**
 * @file fields.hpp
 * @brief Header only library for the fields related routines
 */
#pragma once

#include "mesh.hpp"

namespace lili::mesh {
/**
 * @brief Fields class for electromagnetic fields
 *
 * @details
 * This class is used to store the electromagnetic fields data. The fields data
 * is stored in the Mesh class objects.
 *
 * Currently we store 3D electromagnetic fields for these variables:
 * - Electric fields: \f$\mathbf{E} = (E_x, E_y, E_z)\f$
 * - Magnetic fields: \f$\mathbf{B} = (B_x, B_y, B_z)\f$
 *
 * The relative spacing between the fields points are stored as
 * \f$\mathrm{d}\mathbf{r}_{\mathbf{Q}}\f$ as a multiple of the mesh spacing
 * \f$\mathrm{d}\mathbf{r}\f$.
 *
 * For the Yee grid we have:
 * - \f$E_x\f$ at \f$(0.5, 0, 0)\f$
 * - \f$E_y\f$ at \f$(0, 0.5, 0)\f$
 * - \f$E_z\f$ at \f$(0, 0, 0.5)\f$
 * - \f$B_x\f$ at \f$(0, 0.5, 0.5)\f$
 * - \f$B_y\f$ at \f$(0.5, 0, 0.5)\f$
 * - \f$B_z\f$ at \f$(0.5, 0.5, 0)\f$
 *
 * @note In the next release, current will also be stored.
 */
class Fields {
 public:
  // Constructor
  Fields()
      : dx_(1.0),
        dy_(1.0),
        dz_(1.0),
        dexx_(0.5),
        dexy_(0.0),
        dexz_(0.0),
        deyx_(0.0),
        deyy_(0.5),
        deyz_(0.0),
        dezx_(0.0),
        dezy_(0.0),
        dezz_(0.5),
        dbxx_(0.0),
        dbxy_(0.5),
        dbxz_(0.5),
        dbyx_(0.5),
        dbyy_(0.0),
        dbyz_(0.5),
        dbzx_(0.5),
        dbzy_(0.5),
        dbzz_(0.0) {
    UpdateMeshSizeDim(size);
    InitializeMesh();
  }

  // Size-based initialization
  Fields(int nx, int ny, int nz, int ngx, int ngy, int ngz)
      : dx_(1.0),
        dy_(1.0),
        dz_(1.0),
        dexx_(0.5),
        dexy_(0.0),
        dexz_(0.0),
        deyx_(0.0),
        deyy_(0.5),
        deyz_(0.0),
        dezx_(0.0),
        dezy_(0.0),
        dezz_(0.5),
        dbxx_(0.0),
        dbxy_(0.5),
        dbxz_(0.5),
        dbyx_(0.5),
        dbyy_(0.0),
        dbyz_(0.5),
        dbzx_(0.5),
        dbzy_(0.5),
        dbzz_(0.0) {
    size.nx = nx;
    size.ny = ny;
    size.nz = nz;
    size.ngx = ngx;
    size.ngy = ngy;
    size.ngz = ngz;
    size.lx = nx * 1.0;
    size.ly = ny * 1.0;
    size.lz = nz * 1.0;

    UpdateMeshSizeDim(size);
    InitializeMesh();
  }

  Fields(const MeshSize& domain_size) {
    size = domain_size;
    dx_ = domain_size.lx / domain_size.nx;
    dy_ = domain_size.ly / domain_size.ny;
    dz_ = domain_size.lz / domain_size.nz;

    UpdateMeshSizeDim(size);
    InitializeMesh();
  }

  // Copy constructor
  Fields(const Fields& fields)
      : size(fields.size), dx_(fields.dx_), dy_(fields.dy_), dz_(fields.dz_) {
    UpdateMeshSizeDim(size);
    ex = fields.ex;
    ey = fields.ey;
    ez = fields.ez;
    bx = fields.bx;
    by = fields.by;
    bz = fields.bz;
    SyncSize();
  }

  // Move constructor
  Fields(Fields&& other) noexcept : Fields() { swap(*this, other); };

  // Destructor
  ~Fields() = default;

  // Swap function
  friend void swap(Fields& first, Fields& second) noexcept {
    using std::swap;

    swap(first.dx_, second.dx_);
    swap(first.dy_, second.dy_);
    swap(first.dz_, second.dz_);

    swap(first.size, second.size);

    swap(first.ex, second.ex);
    swap(first.ey, second.ey);
    swap(first.ez, second.ez);
    swap(first.bx, second.bx);
    swap(first.by, second.by);
    swap(first.bz, second.bz);
  };

  // Getters
  /// @cond GETTERS
  constexpr int dim() const { return size.dim; };
  constexpr int nx() const { return size.nx; };
  constexpr int ny() const { return size.ny; };
  constexpr int nz() const { return size.nz; };
  constexpr int ngx() const { return size.ngx; };
  constexpr int ngy() const { return size.ngy; };
  constexpr int ngz() const { return size.ngz; };
  constexpr int ntx() const { return size.nx + 2 * size.ngx; };
  constexpr int nty() const { return size.ny + 2 * size.ngy; };
  constexpr int ntz() const { return size.nz + 2 * size.ngz; };
  constexpr int nt() const { return ntx() * nty() * ntz(); };
  constexpr double dx() const { return dx_; };
  constexpr double dy() const { return dy_; };
  constexpr double dz() const { return dz_; };
  /// @endcond

  void SyncSize() {
    // Sync mesh sizes
    // @todo Implement this to check all sizes
  };

  void InitializeMesh() {
    // Initialize mesh
    ex = Mesh<double>(size.nx, size.ny, size.nz, size.ngx, size.ngy, size.ngz);
    ey = Mesh<double>(size.nx, size.ny, size.nz, size.ngx, size.ngy, size.ngz);
    ez = Mesh<double>(size.nx, size.ny, size.nz, size.ngx, size.ngy, size.ngz);
    bx = Mesh<double>(size.nx, size.ny, size.nz, size.ngx, size.ngy, size.ngz);
    by = Mesh<double>(size.nx, size.ny, size.nz, size.ngx, size.ngy, size.ngz);
    bz = Mesh<double>(size.nx, size.ny, size.nz, size.ngx, size.ngy, size.ngz);

    SyncSize();
  };

  // Public data members
  MeshSize size = {
      1,    // dim
      1,    // nx
      1,    // ny
      1,    // nz
      0,    // ngx
      0,    // ngy
      0,    // ngz
      1.0,  // lx
      1.0,  // ly
      1.0,  // lz
      0.0,  // x0
      0.0,  // y0
      0.0   // z0
  };
  Mesh<double> ex, ey, ez;
  Mesh<double> bx, by, bz;

 private:
  // Private data members
  double dx_, dy_, dz_;  // Mesh spacing
  double dexx_, dexy_, dexz_, deyx_, deyy_, deyz_, dezx_, dezy_, dezz_;
  double dbxx_, dbxy_, dbxz_, dbyx_, dbyy_, dbyz_, dbzx_, dbzy_, dbzz_;
};

void LoadFieldTo(Fields& fields, const char* file_name,
                 bool include_ghost = false);

}  // namespace lili::mesh
