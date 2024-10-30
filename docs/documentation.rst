Documentation
=============

To generate the documentation for this code, build the code with the following command:

.. code-block:: bash

  cmake -DBUILD_DOCS=ON -B build -S .
  cmake --build build

The generated HTML documentation can be found in the ``build/docs/sphinx`` directory.

Doxygen
-------
This code leverage Doxygen to generate automatic documentation for the source code.

Sphinx
------
The main website for the documentation is generated using Sphinx.

Currently, most documentation pages are written in `reStructuredText <https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html>`_ format.

Breathe
-------
Breathe is a Sphinx extension that allows you to include Doxygen documentation into your Sphinx pages.

Exhale
------
Exhale is a helper tool for Breathe that allows you to generate the necessary files to include Doxygen documentation into your Sphinx pages.

Furo
----
The documentation theme is set to Furo. This is a modern and responsive theme for Sphinx documentation.
