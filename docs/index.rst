.. LILI documentation master file, created by
   sphinx-quickstart on Sat Oct  1 11:11:03 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. Copyright (c) 2022, Himawan Winarto

   Distributed under the terms of GNU AGPLv3 License.

   The full license is in the file LICENSE, distributed with this software.

LILI Documentation
====================

**LILI** (*FLuid-KInetic PLasma SImulation Code*) serves as a rapid prototyping platform for fluid and kinetic plasma simulations. The code is designed to be modular, allowing for easy implementation of new physics and numerical methods. Codes are well documented and easy to read, making it a great tool for testing purposes.

The code is written in C++ and is designed to be parallelizable using MPI. Most of the work is done on the scaffolding required to implement the physics and numerical methods.

The code is designed to be trivially vectorized and can be easily transformed into a GPU code.

.. note::

  This code is still in its early stages of development and will be updated frequently.


Contents:
=========

.. toctree::
   :maxdepth: 2

   quickstart
   FAQ <faq>

.. toctree::
  :maxdepth: 4
  :caption: Contents

  physics
  implementation

.. toctree::
  :maxdepth: 3
  :caption: Code documentation

  documentation
  LILI API <sphinx/api/lili_api>

Indices and tables
==================

* :ref:`genindex`
