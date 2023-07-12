#pragma once
/**
 * @file mesh.hpp
 * @brief Header only library for the Mesh related classes
 */

namespace lili::mesh {
/**
 * @brief Enumeration class for the mesh axis
 */
typedef enum {
  None, /**< Irrelevant axis*/
  X,    /**< X-axis */
  Y,    /**< Y-axis */
  Z     /**< Z-axis */
} MeshAxis;

/**
 * @brief Enumeration class for the ghost cell locations
 */
typedef enum {
  XG0, /**< Previous X-axis ghost */
  XG1, /**< Next X-axis ghost */
  YG0, /**< Previous Y-axis ghost */
  YG1, /**< Next Y-axis ghost */
  ZG0, /**< Previous Z-axis ghost */
  ZG1  /**< Next Z-axis ghost */
} MeshGhostLocation;

/**
 * @brief Mesh size struct
 */
typedef struct {
  uint32_t n0, n1, n2;
} MeshSize;

/**
 * @brief Mesh axis index struct
 */
typedef struct {
  MeshAxis ax0;
  uint32_t i0;
  MeshAxis ax1;
  uint32_t i1;
  MeshAxis ax2;
  uint32_t i2;
} MeshAxisIndex;

/**
 * @brief Mesh axis index struct
 */
typedef struct {
  MeshAxis ax0;
  uint32_t i00;
  uint32_t i01;
  MeshAxis ax1;
  uint32_t i10;
  uint32_t i11;
  MeshAxis ax2;
  uint32_t i20;
  uint32_t i21;
} MeshAxisRange;

/**
 * @brief Mesh class
 */
template <typename T>
class Mesh {
 public:
  // Size-based initialization
  Mesh(uint32_t n0);
  Mesh(uint32_t n0, uint32_t n1);
  Mesh(uint32_t n0, uint32_t n1, uint32_t n2);
  Mesh(uint32_t n0, uint32_t n1, uint32_t n2, uint32_t ng);
  Mesh(uint32_t n0, uint32_t n1, uint32_t n2, uint32_t n0g, uint32_t n1g,
       uint32_t n2g);
  Mesh(MeshSize domain_size);
  Mesh(MeshSize domain_size, MeshSize ghost_size);

  // Copy constructor
  Mesh(const Mesh& other);

  // Destructor
  ~Mesh();

  // Getters
  T* data() const { return data_; };
  uint32_t n0() const { return n0_; };
  uint32_t n1() const { return n1_; };
  uint32_t n2() const { return n2_; };
  uint32_t n0g() const { return n0g_; };
  uint32_t n1g() const { return n1g_; };
  uint32_t n2g() const { return n2g_; };
  uint32_t n0t() const { return n0_ + 2 * n0g_; };
  uint32_t n1t() const { return n1_ + 2 * n1g_; };
  uint32_t n2t() const { return n2_ + 2 * n2g_; };
  uint32_t nt() const { return n0t() * n1t() * n2t(); };
  int dim() const { return (n2_ > 1) ? 3 : ((n1_ > 1) ? 2 : 1); };

  // Compare Mesh sizes
  bool SameSizeAs(const Mesh& other);

  // Initialize data
  void InitializeData();

  // Get subset of data
  void GetSubset(Mesh& target, MeshAxisIndex ai, bool include_ghost = false);
  void GetSubset(Mesh& target, MeshAxisRange ar, bool include_ghost = false);

  // Copy data
  void CopyFrom(const Mesh& source);
  void CopyFrom(const Mesh& source, MeshAxisIndex ai_source,
                MeshAxisIndex ai_target);

 private:
  T* data_;  // Pointer to the data block

  uint32_t n0_, n1_, n2_;     // Mesh sizes
  uint32_t n0g_, n1g_, n2g_;  // Ghost cells sizes (same for before and after)
};

// Size-based initialization
template <typename T>
inline Mesh<T>::Mesh(uint32_t n0)
    : data_(nullptr), n0_(n0), n1_(1), n2_(1), n0g_(0), n1g_(0), n2g_(0) {
  InitializeData();
};

template <typename T>
inline Mesh<T>::Mesh(uint32_t n0, uint32_t n1)
    : data_(nullptr), n0_(n0), n1_(n1), n2_(1), n0g_(0), n1g_(0), n2g_(0) {
  InitializeData();
};

template <typename T>
inline Mesh<T>::Mesh(uint32_t n0, uint32_t n1, uint32_t n2)
    : data_(nullptr), n0_(n0), n1_(n1), n2_(n2), n0g_(0), n1g_(0), n2g_(0) {
  InitializeData();
};

template <typename T>
inline Mesh<T>::Mesh(uint32_t n0, uint32_t n1, uint32_t n2, uint32_t ng)
    : data_(nullptr), n0_(n0), n1_(n1), n2_(n2), n0g_(ng), n1g_(ng), n2g_(ng) {
  InitializeData();
};

template <typename T>
inline Mesh<T>::Mesh(uint32_t n0, uint32_t n1, uint32_t n2, uint32_t n0g,
                     uint32_t n1g, uint32_t n2g)
    : data_(nullptr),
      n0_(n0),
      n1_(n1),
      n2_(n2),
      n0g_(n0g),
      n1g_(n1g),
      n2g_(n2g) {
  InitializeData();
};

template <typename T>
inline Mesh<T>::Mesh(MeshSize domain_size)
    : data_(nullptr),
      n0_(domain_size.n0),
      n1_(domain_size.n1),
      n2_(domain_size.n2),
      n0g_(0),
      n1g_(0),
      n2g_(0) {
  InitializeData();
};

template <typename T>
inline Mesh<T>::Mesh(MeshSize domain_size, MeshSize ghost_size)
    : data_(nullptr),
      n0_(domain_size.n0),
      n1_(domain_size.n1),
      n2_(domain_size.n2),
      n0g_(ghost_size.n0),
      n1g_(ghost_size.n1),
      n2g_(ghost_size.n2) {
  InitializeData();
};

// Copy constructor
template <typename T>
inline Mesh<T>::Mesh(const Mesh& other)
    : data_(nullptr),
      n0_(other.n0_),
      n1_(other.n1_),
      n2_(other.n2_),
      n0g_(other.n0g_),
      n1g_(other.n1g_),
      n2g_(other.n2g_) {
  InitializeData();
  CopyFrom(other);
};

// Destructor
template <typename T>
inline Mesh<T>::~Mesh() {
  if (data_ != nullptr) {
    delete[] data_;
  }
};

// Compare Mesh sizes
template <typename T>
inline bool Mesh<T>::SameSizeAs(const Mesh& other) {
  return (n0_ == other.n0_ && n1_ == other.n1_ && n2_ == other.n2_ &&
          n0g_ == other.n0g_ && n1g_ == other.n1g_ && n2g_ == other.n2g_);
}

// Initialize data
template <typename T>
inline void Mesh<T>::InitializeData() {
  // Allocate memory
  data_ = new T[(n0_ + 2 * n0g_) * (n1_ + 2 * n1g_) * (n2_ + 2 * n2g_)]();
};

// Get subset of data
template <typename T>
inline void Mesh<T>::GetSubset(Mesh& target, MeshAxisIndex ai,
                               bool include_ghost) {
  // Calculate the expected output size
  uint32_t nout = 1;
  switch (ai.ax0) {
    case None:
      break;
    case X:
      nout *= ai.i0;
      break;
    case Y:
      nout *= ai.i1;
      break;
    case Z:
      nout *= ai.i2;
      break;
  }

  // Get subset
  for (uint32_t i2 = 0; i2 < ai.i2; i2++) {
    for (uint32_t i1 = 0; i1 < ai.i1; i1++) {
      for (uint32_t i0 = 0; i0 < ai.i0; i0++) {
        target.data[i0 + ai.i0 * (i1 + ai.i1 * i2)] =
            data_[i0 + ai.i0 * (i1 + ai.i1 * i2)];
      }
    }
  }
};

// Copy data
template <typename T>
inline void Mesh<T>::CopyFrom(const Mesh& source) {
  // Check if the sizes are the same
  SameSizeAs(source);

  // Copy data
  std::copy(
      source.data_,
      source.data_ + (n0_ + 2 * n0g_) * (n1_ + 2 * n1g_) * (n2_ + 2 * n2g_),
      data_);
};
}  // namespace lili::mesh