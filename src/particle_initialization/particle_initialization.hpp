/**
 * @file particle_initialization.hpp
 * @brief Header file for particle initialization routines
 */
#pragma once

#include <vector>

#include "mesh.hpp"
#include "particle.hpp"

#ifndef __LILIP_DEFAULT_GTSIZE
#define __LILIP_DEFAULT_GTSIZE 10000
#endif

#ifndef __LILIP_DEFAULT_GTMULT
#define __LILIP_DEFAULT_GTMULT 20
#endif

namespace lili::particle {
void DistributeID(Particles& particles, const int offset);
void DistributeLocationUniform(Particles& particles, const int seed,
                               const double x0, const double x1,
                               const double y0, const double y1,
                               const double z0, const double z1);
void DistributeLocationUniform(Particles& particles, const int seed,
                               const mesh::MeshSize& domain_size);

// Gamma table
class GammaTable {
 public:
  GammaTable();
  GammaTable(std::vector<double> cdf, std::vector<double> gamma);

  // Getters
  bool interpolate() const { return interpolate_; }
  int capacity() const { return capacity_; }
  double cdf(const int i) const { return cdf_[i]; }
  double gamma(const int i) const { return gamma_[i]; }

  // Setters
  bool& interpolate() { return interpolate_; }

  // Function to get gamma from CDF
  double GetGamma(const double cdf) const;

 private:
  bool interpolate_;
  int capacity_;
  std::vector<double> cdf_;
  std::vector<double> gamma_;
};

// Gamma table initialization
GammaTable GTMonoenergetic(const double delta_gamma);
GammaTable GTUniform(const double gamma_min, const double gamma_max);
GammaTable GTMaxwellian3D(const double theta);

// Velocity initialization
void AddBulkVelocity(Particles& particles, const double u, const double v,
                     const double w);
void DistributeVelocityUniform(Particles& particles, const int seed,
                               GammaTable& energy_table);

}  // namespace lili::particle