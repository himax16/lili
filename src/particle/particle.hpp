#pragma once
/**
 * @file mesh.hpp
 * @brief Header file for the Particle class
 */

namespace particle {
class Particle;
void copy_particle(Particle* input, Particle* output);
void resize_particle(Particle* input, int len);
void reorder_particle(Particle* input);
}  // namespace particle
