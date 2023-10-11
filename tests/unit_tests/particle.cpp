// Test particles
int npar = 10;
lili::particle::Particles particles(npar);

for (int i = 0; i < npar; ++i) {
  particles.id(i) = i;
  particles.x(i) = i;
  particles.y(i) = i;
  particles.z(i) = i;
  particles.u(i) = i;
  particles.v(i) = i;
  particles.w(i) = i;
}

particles.pswap(1, 2);

particles.status(3) = lili::particle::ParticleStatus::Out;
particles.status(8) = lili::particle::ParticleStatus::Out;
particles.clean_out();

particles.npar() = 15;

lili::particle::save_particles(particles, "test.h5");

lili::particle::Particles particles2 =
    lili::particle::load_particles("test.h5");

particles2.clean_out();
lili::particle::save_particles(particles2, "test2.h5");

// Test particles
int npar = input.particles()[0].n;
particle::Particles particles(npar);
// lili::particle::GammaTable table = particle::GTMonoenergetic(1.25);
// lili::particle::GammaTable table = particle::GTUniform(1., 1.25);
particle::GammaTable table = particle::GTMaxwellian3D(0.36);

particle::DistributeLocationUniform(particles, 0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
particle::DistributeVelocityUniform(particles, 0, table);

particle::SaveParticles(particles, "test.h5");

// Randomly set particle status
particles[0].status(2) = particle::ParticleStatus::Tracked;
particles[0].status(7) = particle::ParticleStatus::Tracked;
particles[0].status(9) = particle::ParticleStatus::Tracked;

// Randomly move particles
particles[0].x(0) = -2.0;
particles[0].y(0) = -2.0;
particles[0].z(0) = -2.0;
particles[0].x(1) = 2.0;
particles[0].y(1) = -2.0;
particles[0].z(7) = -2.0;

// Reclassify particles
particle::LabelBoundaryParticles(particles[0], input.mesh());

// Print particle status
for (int i = 0; i < particles[0].npar(); ++i) {
  if (particles[0].status(i) != particle::ParticleStatus::In) {
    std::cout << "Par " << i << " "
              << particle::__LILIP_SNAME[particles[0].status(i)] << std::endl;
  }
}