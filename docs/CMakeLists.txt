# Documentation
message(STATUS "Enabling documentation")

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  # TODO: Run the Doxygen whenever the source files change. This is a bit
  # tricky because we need to know the source files status without actually
  # building the project. Problem with add_custom_command(...) not allowing
  # different directory for the TARGET.

  set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
  set(DOXYGEN_STRIP_FROM_PATH ${DOXYGEN_INPUT_DIR})
  set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
  set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # Replace the Doxygen.in variables
  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
  message(STATUS "Configuring Doxygen input")

  # Make the Doxygen output diretory
  file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

  # Only regenerate Doxygen when Doxyfile or files change
  add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                     MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                     COMMENT "Generating documentation with Doxygen"
                     VERBATIM)

  add_custom_target(Doxygen_LILI ALL DEPENDS ${DOXYGEN_INDEX_FILE})
else(DOXYGEN_FOUND)
  message(FATAL_ERROR "Missing Doxygen")
endif(DOXYGEN_FOUND)

# Sphinx
find_package(Sphinx)
if(SPHINX_FOUND)
  set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
  set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
  set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)
  set(SPHINX_BUILD_RST_FILES ${SPHINX_BUILD}/index.rst)

  set(BREATHE_DOXYGEN_XML ${DOXYGEN_OUTPUT_DIR}/xml)
  set(EXHALE_CONTAINMENT_FOLDER ${SPHINX_BUILD}/api)

  # Configure Sphinx config file
  set(SPHINX_CONF_IN ${SPHINX_SOURCE}/conf.py.in)
  set(SPHINX_CONF_OUT ${CMAKE_CURRENT_BINARY_DIR}/conf.py)

  # Replace the conf.py.in variables
  configure_file(${SPHINX_CONF_IN} ${SPHINX_CONF_OUT} @ONLY)
  message(STATUS "Configuring Sphinx input")

  # Copy all of the rst files
  file(GLOB_RECURSE
    SPHINX_RST_FILES
    RELATIVE ${SPHINX_SOURCE}
    CONFIGURE_DEPENDS
    ${SPHINX_SOURCE}/*.rst
  )
  FOREACH(SPHINX_RST_FILE ${SPHINX_RST_FILES})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SPHINX_RST_FILE}
      COMMAND ${CMAKE_COMMAND} -E copy
        ${SPHINX_SOURCE}/${SPHINX_RST_FILE}
        ${CMAKE_CURRENT_BINARY_DIR}/${SPHINX_RST_FILE}
      DEPENDS ${SPHINX_SOURCE}/${SPHINX_RST_FILE}
    )
  ENDFOREACH()

  list(TRANSFORM SPHINX_RST_FILES PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)

  # Only regenerate Sphinx when:
  # - Doxygen has rerun
  # - Our doc files have been updated
  # - The Sphinx config has been updated
  add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                     COMMAND
                       ${SPHINX_EXECUTABLE} -b html
                       ${CMAKE_CURRENT_BINARY_DIR} ${SPHINX_BUILD}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     DEPENDS
                     # Other docs files you want to track should go here
                     # (or in some variable)
                       ${SPHINX_RST_FILES}
                       ${DOXYGEN_INDEX_FILE}
                     MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py.in
                     COMMENT "Generating documentation with Sphinx")

  add_custom_target(Sphinx_LILI ALL
    DEPENDS ${SPHINX_RST_FILES} ${SPHINX_INDEX_FILE}
  )

  # Add an install target to install the docs
  include(GNUInstallDirs)
  install(DIRECTORY ${SPHINX_BUILD}
          DESTINATION ${CMAKE_INSTALL_DOCDIR})
else(SPHINX_FOUND)
  message(FATAL_ERROR "Missing Sphinx")
endif(SPHINX_FOUND)
