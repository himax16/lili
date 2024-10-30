Quickstart
==========

Get LILI from the `GitHub repository <https://github.com/himax16/lili>`_ and clone it to your local machine.

.. code-block:: bash

  git clone https://github.com/himax16/lili.git

Installation
------------

LILI uses `Spack <https://spack.io>`_ to manage the dependencies. To install Spack, follow the instructions on the `Spack installation page <https://spack.readthedocs.io/en/latest/getting_started.html>`_.

Once Spack is installed, you can install the dependencies for LILI by running the following command from the root directory of the repository:

.. code-block:: bash

  spack env activate .
  spack install

.. note::

  If you are running on a cluster, please make sure to load all of the necessary modules before running the above command so that Spack can use the optimized versions of the libraries.

This will install all of the necessary dependencies for LILI. This process can take a while depending on the number of dependencies that need to be installed.

.. attention::

  The documentation is currently built using Exhale *v0.36* with newer version of Sphinx, required for using the Furo theme. To trick the Spack environment dependencies, we need to install ``py-sphinx-rtd-theme`` temporarily.

  This will be fixed in the future.

Building
--------

To build LILI, run the following commands:

.. code-block:: bash

  cmake -B build -S lili
  cmake --build build

The compiled binary will be located at ``build/lili``.

Running
-------

To run LILI, use the following command:
