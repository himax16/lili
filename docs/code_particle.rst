Particle
========

The :class:`lili::particle::Particles` stores the particle data in the simualtion. Currently, the position of the particles are stored in the local mesh coordinate.

Initialization
--------------

Particle distribution in the simulation are initialized at the start of the simulation using :class:`lili::task::TaskInitParticles`.

Position distribution
^^^^^^^^^^^^^^^^^^^^^

Currently there are two types of particle distribution implemented:

**Stationary distribution**

  The particles are left at their initial position at :math:`(0,0,0)`.

**Uniform distribution**

  The particles are distributed uniformly in the domain.
