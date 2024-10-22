/**
 * @file particle_initialization.cpp
 * @brief Source file for particle initialization routines
 * @details
 * This file contains the implementation of the particle initialization
 * routines.
 * @todo
 * - Add compiler independent random number generator
 */

#include "particle_initialization.hpp"

#include <cmath>
#include <random>

namespace lili::particle {
/**
 * @brief Distribute particle IDs
 * @param[in] particles
 * Particle object
 * @param[in] offset
 * Offset for the particle IDs
 */
void DistributeID(Particles& particles, const int offset) {
  for (int i = 0; i < particles.npar(); ++i) {
    particles.id(i) = i + offset;
  }
}

/**
 * @brief Distribute particles uniformly in the simulation domain
 * @param[in] particles
 * Particle object
 * @param[in] seed
 * Seed for the random number generator
 * @param[in] x0
 * Minimum value of the X coordinate
 * @param[in] x1
 * Maximum value of the X coordinate
 * @param[in] y0
 * Minimum value of the Y coordinate
 * @param[in] y1
 * Maximum value of the Y coordinate
 * @param[in] z0
 * Minimum value of the Z coordinate
 * @param[in] z1
 * Maximum value of the Z coordinate
 */
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

/**
 * @brief Distribute particles uniformly in the simulation domain
 * @param[in] particles
 * Particle object
 * @param[in] seed
 * Seed for the random number generator
 * @param[in] domain_size
 * Mesh size object
 */
void DistributeLocationUniform(Particles& particles, const int seed,
                               const mesh::MeshSize& domain_size) {
  double x1 = domain_size.x0 + domain_size.lx;
  double y1 = domain_size.y0 + domain_size.ly;
  double z1 = domain_size.z0 + domain_size.lz;

  DistributeLocationUniform(particles, seed, domain_size.x0, x1, domain_size.y0,
                            y1, domain_size.z0, z1);
}

/**
 * @brief Add bulk velocity to particles
 * @param[in] particles
 * Particle object
 * @param[in] u
 * Bulk velocity in the X direction
 * @param[in] v
 * Bulk velocity in the Y direction
 * @param[in] w
 * Bulk velocity in the Z direction
 */
void AddBulkVelocity(Particles& particles, const double u, const double v,
                     const double w) {
  for (int i = 0; i < particles.npar(); ++i) {
    particles.u(i) += u;
    particles.v(i) += v;
    particles.w(i) += w;
  }
}

/**
 * @brief Distribute particle energy uniformly
 * @param[in] particles
 * Particle object
 * @param[in] seed
 * Seed for the random number generator
 * @param[in] energy_table
 * Gamma table
 */
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

/**
 * @brief Construct a new Gamma Table:: Gamma Table object
 */
GammaTable::GammaTable() {
  interpolate_ = true;
  capacity_ = 0;

  cdf_.resize(0);
  gamma_.resize(0);
}

/**
 * @brief Construct a new Gamma Table:: Gamma Table object
 * @param[in] cdf
 * CDF values
 * @param[in] gamma
 * Gamma values
 */
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

/**
 * @brief Get gamma from CDF
 * @param[in] cdf
 * CDF value
 * @return double
 * Gamma value
 */
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

/**
 * @brief Construct a new Gamma Table for monoenergetic particles
 * @param[in] delta_gamma
 * Delta gamma value
 * @return GammaTable
 * Gamma table
 */
GammaTable GTMonoenergetic(const double delta_gamma) {
  std::vector<double> cdf = {0.0, 1.0};
  std::vector<double> gamma_vec = {1. + delta_gamma, 1. + delta_gamma};

  GammaTable gamma_table(cdf, gamma_vec);
  gamma_table.interpolate() = false;

  return gamma_table;
}

/**
 * @brief Construct a new Gamma Table for particles with uniform energy
 * @param[in] gamma_min
 * Minimum gamma value
 * @param[in] gamma_max
 * Maximum gamma value
 * @return GammaTable
 * Gamma table
 */
GammaTable GTUniform(const double gamma_min, const double gamma_max) {
  std::vector<double> cdf = {0.0, 1.0};
  std::vector<double> gamma_vec = {gamma_min, gamma_max};

  return GammaTable(cdf, gamma_vec);
}

/**
 * @brief Construct a new Gamma Table for particles with 3D Maxwellian energy
 * distribution
 * @details
 * Create a table of gamma values for particles with a Maxwellian energy
 * satisfying isotropic Maxwell-Juttner distribution:
 * @f[
 * f(\gamma) \sim \gamma (\gamma^2 - 1)^{\frac{d}{2} - 1}  e^{-\gamma/\theta}
 * @f]
 * where @f$ d @f$ is the number of spatial dimensions and @f$ \theta @f$ is the
 * temperature parameter.
 * @param[in] theta
 * Temperature parameter
 * @return GammaTable
 * Gamma table
 */
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
