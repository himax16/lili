Particle Motion
===============

For a charged particle with mass :math:`m` and charge :math:`q` in the presence of electromagnetic fields, the equation of motion (EOM) is given by:

.. math::

    \frac{\mathrm{d} \mathbf{r}}{\mathrm{d} t} &= \mathbf{v},\\
    \frac{\mathrm{d} \mathbf{v}}{\mathrm{d} t} &= \frac{q}{m} \left( \mathbf{E} + \frac{\mathbf{v}}{c} \times \mathbf{B} \right).

Here we have defined the particle's position :math:`\mathbf{r}` and velocity :math:`\mathbf{v}`. The electric field is :math:`\mathbf{E}` and the magnetic field is :math:`\mathbf{B}` are measured at the particle's position.

The equation of motion can be solved using a variety of numerical methods. The most common method is the **Boris pusher**, which is a second-order accurate method for solving the equation of motion.

Boris Pusher
------------

Boris pusher is a second-order accurate symplectic integrator for solving the particle EOM. In order to use the Boris pusher, we define the particle velocity at half time steps :math:`\mathbf{v}^{n + 1/2}` and particle position at full time steps :math:`\mathbf{r}^{n}`.

The velocity is updated using:

.. math::

    \mathbf{v}^{n + 1/2} &= \mathbf{v}^n + \frac{q}{2 m} \mathbf{E} \Delta t,\\
    \mathbf{v}^{n + 1} &= \mathbf{v}^{n + 1/2} + \frac{q}{m} \left( \mathbf{v}^{n + 1/2} \times \mathbf{B} \right) \Delta t,\\
    \mathbf{v}^{n + 3/2} &= \mathbf{v}^{n + 1} + \frac{q}{2 m} \mathbf{E} \Delta t.

The position can then be updated trivially using:

.. math::

    \mathbf{r}^{n + 1} = \mathbf{r}^n + \mathbf{v}^{n + 1} \Delta t.
