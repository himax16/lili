/**
 * @file mesh.hpp
 * @brief Header only library for the Mesh related classes
 */
#pragma once

#include <algorithm>
#include <cstdint>

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

  // Move constructor
  Mesh(Mesh&& other) noexcept : Mesh() { swap(*this, other); };

  // Destructor
  ~Mesh();

  // Getters
  constexpr T* data() const { return data_; };
  constexpr uint32_t n0() const { return n0_; };
  constexpr uint32_t n1() const { return n1_; };
  constexpr uint32_t n2() const { return n2_; };
  constexpr uint32_t n0g() const { return n0g_; };
  constexpr uint32_t n1g() const { return n1g_; };
  constexpr uint32_t n2g() const { return n2g_; };
  constexpr uint32_t n0t() const { return n0_ + 2 * n0g_; };
  constexpr uint32_t n1t() const { return n1_ + 2 * n1g_; };
  constexpr uint32_t n2t() const { return n2_ + 2 * n2g_; };
  constexpr uint32_t nt() const { return n0t() * n1t() * n2t(); };
  constexpr int dim() const { return (n2_ > 1) ? 3 : ((n1_ > 1) ? 2 : 1); };

  // Operators
  Mesh<T>& operator=(Mesh<T> other) {
    swap(*this, other);
    return *this;
  };

  T operator()(uint32_t i0) const { return data_[i0]; };
  T& operator()(uint32_t i0) { return data_[i0]; };

  T operator()(uint32_t i0, uint32_t i1) const { return data_[i0 + n0_ * i1]; };
  T& operator()(uint32_t i0, uint32_t i1) { return data_[i0 + n0_ * i1]; };

  T operator()(uint32_t i0, uint32_t i1, uint32_t i2) const {
    return data_[i0 + n0_ * (i1 + n1_ * i2)];
  };
  T& operator()(uint32_t i0, uint32_t i1, uint32_t i2) {
    return data_[i0 + n0_ * (i1 + n1_ * i2)];
  };

  // Compare Mesh sizes
  bool SameSizeAs(const Mesh& other);

  // Initialize data
  void InitializeData();

  // Swap data
  friend void swap(Mesh<T>& first, Mesh<T>& second) noexcept {
    using std::swap;
    swap(first.data_, second.data_);
    swap(first.n0_, second.n0_);
    swap(first.n1_, second.n1_);
    swap(first.n2_, second.n2_);
    swap(first.n0g_, second.n0g_);
    swap(first.n1g_, second.n1g_);
    swap(first.n2g_, second.n2g_);
  }

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
  std::copy(other.data_, other.data_ + other.nt(), data_);
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
}  // namespace lili::mesh