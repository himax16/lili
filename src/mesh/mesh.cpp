#ifndef LILI_MESH_HPP_
#define LILI_MESH_HPP_

template <typename T>
class Mesh {
 private:
  T* data_;
  int n0_, n1_, n2_;
  int n0g_, n1g_, n2g_;

 public:
  // Empty constructor
  Mesh() : data_(nullptr), n0_(0), n1_(0), n2_(0), n0g_(0), n1g_(0), n2g_(0){};

  // Size-based initialization
  Mesh(int n0)
      : data_(nullptr), n0_(n0), n1_(1), n2_(1), n0g_(0), n1g_(0), n2g_(0) {
    init_data()
  }
  
  Mesh(int n0, int n1)
      : data_(nullptr), n0_(0), n1_(0), n2_(0), n0g_(0), n1g_(0), n2g_(0) {
    init_data()
  }
}

#endif