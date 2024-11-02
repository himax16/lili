# Check if json.hpp is not in the current source directory
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
  message(STATUS "Downloading json.hpp from the nlohmann/json repository")

  # Set the download address
  set(JSON_DL_URL https://raw.githubusercontent.com/nlohmann/json/refs/heads/develop/single_include/nlohmann/json.hpp)

  # Download json.hpp from the nlohmann/json repository
  file(DOWNLOAD ${JSON_DL_URL} ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)

  # Check if the download was successful
  if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)
    message(FATAL_ERROR "Failed to download json.hpp")
  else()
    # Check if the file contains #define INCLUDE_NLOHMANN_JSON_HPP_
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp JSON_HPP_CONTENTS)

    string(FIND "${JSON_HPP_CONTENTS}"
           "#define INCLUDE_NLOHMANN_JSON_HPP_" JSON_HPP_FOUND)

    if (${JSON_HPP_FOUND} EQUAL -1)
      # Remove the downloaded file
      file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp)

      # Print an error message
      message(FATAL_ERROR "Downloaded json.hpp is not valid")
    endif()
  endif()
endif()

add_library(json INTERFACE)
target_include_directories(json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
