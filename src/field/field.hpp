/**
 * @file field.hpp
 * @brief Header only library for the field related routines
 */
#pragma once

#include "mesh.hpp"

/**
 * @brief Field class representing a 3D mesh field.
 *
 * The Field class provides various constructors for initializing the mesh field
 * with different parameters, including default, size-based, and domain
 * size-based initializations. It also includes copy and move constructors, as
 * well as a destructor.
 *
 * The class contains methods for getting mesh dimensions and spacings,
 * synchronizing mesh sizes, and initializing the mesh. It also provides a swap
 * function for swapping two Field objects.
 *
 * Public Data Members:
 * - MeshSize size: Structure containing mesh size and domain information.
 * - Mesh<double> ex, ey, ez: Meshes for electric field components.
 * - Mesh<double> bx, by, bz: Meshes for magnetic field components.
 *
 * Private Data Members:
 * - double dx_, dy_, dz_: Mesh spacing in x, y, and z directions.
 *
 * Methods:
 * - Field(): Default constructor.
 * - Field(int nx, int ny, int nz, int ngx, int ngy, int ngz): Size-based
 * initialization.
 * - Field(const MeshSize& domain_size): Domain size-based initialization.
 * - Field(const Field& field): Copy constructor.
 * - Field(Field&& other) noexcept: Move constructor.
 * - ~Field(): Destructor.
 * - friend void swap(Field& first, Field& second) noexcept: Swap function.
 * - constexpr int dim() const: Get mesh dimension.
 * - constexpr int nx() const: Get number of cells in x direction.
 * - constexpr int ny() const: Get number of cells in y direction.
 * - constexpr int nz() const: Get number of cells in z direction.
 * - constexpr int ngx() const: Get number of ghost cells in x direction.
 * - constexpr int ngy() const: Get number of ghost cells in y direction.
 * - constexpr int ngz() const: Get number of ghost cells in z direction.
 * - constexpr int ntx() const: Get total number of cells in x direction
 * (including ghost cells).
 * - constexpr int nty() const: Get total number of cells in y direction
 * (including ghost cells).
 * - constexpr int ntz() const: Get total number of cells in z direction
 * (including ghost cells).
 * - constexpr int nt() const: Get total number of cells (including ghost
 * cells).
 * - constexpr double dx() const: Get mesh spacing in x direction.
 * - constexpr double dy() const: Get mesh spacing in y direction.
 * - constexpr double dz() const: Get mesh spacing in z direction.
 * - void SyncSize(): Synchronize mesh sizes.
 * - void InitializeMesh(): Initialize the mesh.
 *
 * @file field.hpp
 * @namespace lili::mesh
 */
namespace lili::mesh {
/**
 * @brief Field class
 */
class Field {
 public:
  // Constructor
  Field() : dx_(1.0), dy_(1.0), dz_(1.0) {
    UpdateMeshSizeDim(size);
    InitializeMesh();
  };

  // Size-based initialization
  Field(int nx, int ny, int nz, int ngx, int ngy, int ngz)
      : dx_(1.0), dy_(1.0), dz_(1.0) {
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
  };

  Field(const MeshSize& domain_size) {
    size = domain_size;
    dx_ = domain_size.lx / domain_size.nx;
    dy_ = domain_size.ly / domain_size.ny;
    dz_ = domain_size.lz / domain_size.nz;

    UpdateMeshSizeDim(size);
    InitializeMesh();
  };

  // Copy constructor
  Field(const Field& field)
      : size(field.size), dx_(field.dx_), dy_(field.dy_), dz_(field.dz_) {
    UpdateMeshSizeDim(size);
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
  MeshSize size = {.dim = 1,
                   .nx = 1,
                   .ny = 1,
                   .nz = 1,
                   .ngx = 0,
                   .ngy = 0,
                   .ngz = 0,
                   .lx = 1.0,
                   .ly = 1.0,
                   .lz = 1.0,
                   .x0 = 0.0,
                   .y0 = 0.0,
                   .z0 = 0.0};
  Mesh<double> ex, ey, ez;
  Mesh<double> bx, by, bz;

 private:
  // Private data members
  double dx_, dy_, dz_;  // Mesh spacing
};

void LoadFieldTo(Field& field, const char* file_name,
                 bool include_ghost = false);

}  // namespace lili::mesh
