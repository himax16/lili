Documentation
=============

.. note::

  The documentation for this code can be accessed online at `lili.hima.id <https://lili.hima.id>`_.

To generate the documentation for this code, build the code with the following command:

.. code-block:: bash

  cmake -DBUILD_DOCS=ON -B build -S .
  cmake --build build

The generated HTML documentation can be found in the ``build/docs/sphinx`` directory.

Doxygen
-------

This code leverage Doxygen to generate automatic documentation for the source code. The Doxygen configuration file is located at ``docs/Doxyfile.in``, with some variables are configured by CMake during the build process.

Sphinx
------

The website of the documentation is generated using `Sphinx <https://www.sphinx-doc.org/en/master/>`_. Most documentation pages are written in `reStructuredText <https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html>`_ format and can be found in the ``docs`` directory.

The configuration for Sphinx can be found in the ``docs/conf.py.in`` file.

Breathe
-------

Given that Sphinx does not natively support Doxygen, we use `Breathe <https://breathe.readthedocs.io/en/latest/>`_ to parse the Doxygen XML files and include them in the Sphinx pages.

The Breathe configuration can be found within the Sphinx configuration file.

Exhale
------

Exhale is another helper tool that we use to automatically generate the API documentation from the Doxygen XML files.

The Exhale configuration can be found within the Sphinx configuration file.

Uploading Documentation
-----------------------

Built documentation can be uploaded to a GitHub repository to be hosted on GitHub Pages. You can use the attached Bash script ``upload_docs.sh`` to upload the documentation.
