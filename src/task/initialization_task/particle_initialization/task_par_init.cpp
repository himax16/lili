/**
 * @file task_par_init.cpp
 * @brief Source file for particle initialization routines
 * @details
 * This file contains the implementation of the particle initialization
 * routines.
 * @todo
 * - Add compiler independent random number generator
 */

#include <cmath>
#include <random>

#include "particle_initialization.hpp"

namespace lili::particle {
void DistributeID(Particles& particles, const int offset) {
  for (int i = 0; i < particles.npar(); ++i) {
    particles.id(i) = i + offset;
  }
}

void DistributeLocationUniform(Particles& particles, const int seed,
                               const double x0, const double x1,
                               const double y0, const double y1,
                               const double z0, const double z1) {
  // Initialize random number generator
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> dis_x(x0, x1);
  std::uniform_real_distribution<double> dis_y(y0, y1);
  std::uniform_real_distribution<double> dis_z(z0, z1);

  // Distribute particles
  for (int i = 0; i < particles.npar(); ++i) {
    particles.x(i) = dis_x(gen);
    particles.y(i) = dis_y(gen);
    particles.z(i) = dis_z(gen);
  }
}

void DistributeLocationUniform(Particles& particles, const int seed,
                               const mesh::MeshSize& domain_size) {
  double x1 = domain_size.x0 + domain_size.lx;
  double y1 = domain_size.y0 + domain_size.ly;
  double z1 = domain_size.z0 + domain_size.lz;

  DistributeLocationUniform(particles, seed, domain_size.x0, x1, domain_size.y0,
                            y1, domain_size.z0, z1);
}

void AddBulkVelocity(Particles& particles, const double u, const double v,
                     const double w) {
  for (int i = 0; i < particles.npar(); ++i) {
    particles.u(i) += u;
    particles.v(i) += v;
    particles.w(i) += w;
  }
}

void DistributeVelocityUniform(Particles& particles, const int seed,
                               GammaTable& energy_table) {
  // Initialize random number generator
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> dis(0.0, 1.0);

  // Distribute velocities
  for (int i = 0; i < particles.npar(); ++i) {
    double r = dis(gen);
    double gamma = energy_table.GetGamma(r);
    double gammabeta = std::sqrt(gamma * gamma - 1.0);

    double costheta = dis(gen) * 2.0 - 1.0;
    double sintheta = std::sqrt(1.0 - costheta * costheta);
    double phi = dis(gen) * 2.0 * M_PI;

    particles.u(i) = gammabeta * std::cos(phi) * sintheta;
    particles.v(i) = gammabeta * std::sin(phi) * sintheta;
    particles.w(i) = gammabeta * costheta;
  }
}

GammaTable::GammaTable() {
  interpolate_ = true;
  capacity_ = 0;

  cdf_.resize(0);
  gamma_.resize(0);
}

GammaTable::GammaTable(std::vector<double> cdf, std::vector<double> gamma) {
  interpolate_ = true;
  capacity_ = cdf.size();

  cdf_.resize(capacity_);
  gamma_.resize(capacity_);

  for (int i = 0; i < capacity_; ++i) {
    cdf_[i] = cdf[i];
    gamma_[i] = gamma[i];
  }
}

double GammaTable::GetGamma(const double cdf) const {
  // Binary search
  int i_min = 0;
  int i_max = capacity_ - 1;
  int i_mid = 0;

  while (i_max - i_min > 1) {
    i_mid = (i_min + i_max) / 2;

    if (cdf_[i_mid] > cdf) {
      i_max = i_mid;
    } else {
      i_min = i_mid;
    }
  }

  // Interpolate if needed
  if (interpolate_) {
    return gamma_[i_min] + (gamma_[i_max] - gamma_[i_min]) *
                               (cdf - cdf_[i_min]) /
                               (cdf_[i_max] - cdf_[i_min]);
  }

  return gamma_[i_min];
}

GammaTable GTMonoenergetic(const double delta_gamma) {
  std::vector<double> cdf = {0.0, 1.0};
  std::vector<double> gamma_vec = {1. + delta_gamma, 1. + delta_gamma};

  GammaTable gamma_table(cdf, gamma_vec);
  gamma_table.interpolate() = false;

  return gamma_table;
}

GammaTable GTUniform(const double gamma_min, const double gamma_max) {
  std::vector<double> cdf = {0.0, 1.0};
  std::vector<double> gamma_vec = {gamma_min, gamma_max};

  return GammaTable(cdf, gamma_vec);
}

GammaTable GTMaxwellian3D(const double theta) {
  // Initialize maximum delta gamma
  double dgamma_max = theta * __LILIP_DEFAULT_GTMULT;

  // Initialize vectors
  int capacity = __LILIP_DEFAULT_GTSIZE;
  std::vector<double> pdf(capacity);
  std::vector<double> cdf(capacity);
  std::vector<double> gamma(capacity);

  // Construct the gamma table as linearly spaced delta gamma
  for (int i = 0; i < capacity; ++i) {
    gamma[i] = dgamma_max * i / (capacity - 1);
  }

  // Construct the PDF table
  for (int i = 1; i < capacity; ++i) {
    pdf[i] = (gamma[i] + 1.0) * std::sqrt(gamma[i] * (gamma[i] + 2.0)) *
             std::exp(-gamma[i] / theta);
  }

  // Construct the CDF table
  cdf[0] = 0.0;
  for (int i = 1; i < capacity; ++i) {
    cdf[i] = cdf[i - 1] + pdf[i];
  }
  for (int i = 0; i < capacity; ++i) {
    cdf[i] /= cdf[capacity - 1];
  }

  // Change back gamma array from delta gamma
  for (int i = 0; i < capacity; ++i) {
    gamma[i] += 1.0;
  }

  return GammaTable(cdf, gamma);
}
}  // namespace lili::particle
