Code Implementation
===================

.. toctree::
    :maxdepth: 2
    :hidden:

    code_particle

LILI is designed to be a flexible framework to accomodate different types of simulations.

**Modular**
  The goal for LILI is to have a multiscale capabilities through implementation of MHD, two fluids, hybrid, and PIC.

  To achieve this, routines of the codes are divided into **Tasks** which can be extended easily. This task is done with the help of modules written under :ref:`namespace_lili__task`. The goal for this is to have non-blocking routines that can efficiently uses the CPU and GPU.

  Quantities in the codes are stored in class objects, such as :class:`Particles <lili::particle::Particles>` and :class:`Fields <lili::mesh::Fields>`, which can be easily extended. This design choice means that the routines needs to have an explicit reference to the quantities that they are working on rather than relying on global variables.

  Base classes can be easily extended to implement new physics or numerical methods.

**Modern I/O**
  LILI uses the ``JSON`` format for the input file and ``HDF5`` format for the large output or restart files.

  The JSON parser used is based on the header-only library `nlohmann/json <https://github.com/nlohmann/json>`_.

**Documentized**
  All of the codes in LILI should be well documented and easy to read. The aim is to lower the barrier of entry for new users and developers for adapting and improving the code.

  For more information on the code documentation, see :doc:`documentation`.

**Vectorized**
  The data layout in LILI is designed to be easily vectorized such that the code can be easily transformed into a GPU code.
