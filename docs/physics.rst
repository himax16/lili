Physics
=======

.. toctree::
    :maxdepth: 2
    :hidden:

    particle_eom

**LILI** is designed to be modular with different physics implemented as separate modules. To achieve this, some design choices are required to maintain interoperability between different physics modules.

Physical units
--------------

LILI uses **Gaussian units** for all physical quantities with further normalizations given by:

.. list-table::
   :widths: 20 10 10 10
   :header-rows: 1

   * - Quantity
     - Physical symbol
     - Code symbol
     - Value
   * - Speed of light
     - :math:`c`
     - `c`
     - :math:`1`
   * - Electron charge
     - :math:`e`
     - `e`
     - :math:`1`
   * - Electron mass
     - :math:`m_e`
     - `m_e`
     - :math:`1`
