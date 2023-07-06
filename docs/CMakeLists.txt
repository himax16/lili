# Documentation
option(BUILD_DOC "Creating documentation" ON)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  # Configure the Doxygen file
  get_target_property(LILI_SRC_DIR lili INTERFACE_INCLUDE_DIRECTORIES)
  file(GLOB_RECURSE LILI_SRC_FILES
       ${LILI_SRC_DIR}/*.h
       ${LILI_SRC_DIR}/*.hpp
       ${LILI_SRC_DIR}/*.cpp)

  set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
  set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
  set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # Replace the Doxygen.in variables
  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
  message("Doxygen build started")

  # Make the Doxygen output diretory
  file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

  # Only regenerate Doxygen when Doxyfile or files change
  add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                     DEPENDS ${LILI_SRC_FILES}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                     MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                     COMMENT "Generating documentation with Doxygen"
                     VERBATIM)

  add_custom_target(Doxygen_LILI ALL DEPENDS ${DOXYGEN_INDEX_FILE})
else(DOXYGEN_FOUND)
  message("Missing Doxygen")
endif(DOXYGEN_FOUND)

# Sphinx
find_package(Sphinx)
if(SPHINX_FOUND)
  set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
  set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
  set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

  # Only regenerate Sphinx when:
  # - Doxygen has rerun
  # - Our doc files have been updated
  # - The Sphinx config has been updated
  add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                     COMMAND 
                       ${SPHINX_EXECUTABLE} -b html
                       # Tell Breathe where to find the Doxygen output
                       -Dbreathe_projects.LILI=${DOXYGEN_OUTPUT_DIR}/xml
                     ${SPHINX_SOURCE} ${SPHINX_BUILD}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     DEPENDS
                     # Other docs files you want to track should go here
                     # (or in some variable)
                       ${SPHINX_SOURCE}/index.rst
                       ${DOXYGEN_INDEX_FILE}
                     MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                     COMMENT "Generating documentation with Sphinx")

  add_custom_target(Sphinx_LILI ALL DEPENDS ${SPHINX_INDEX_FILE})

  # Add an install target to install the docs
  include(GNUInstallDirs)
  install(DIRECTORY ${SPHINX_BUILD}
          DESTINATION ${CMAKE_INSTALL_DOCDIR})
else(SPHINX_FOUND)
  message("Missing Sphinx")
endif(SPHINX_FOUND)