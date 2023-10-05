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