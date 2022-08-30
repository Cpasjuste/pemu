cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 14)

# Normalize backslashes from Windows paths
file(TO_CMAKE_PATH "${CMAKE_MODULE_PATH}" CMAKE_MODULE_PATH)
file(TO_CMAKE_PATH "${CMAKE_PREFIX_PATH}" CMAKE_PREFIX_PATH)
message(STATUS "Module Path: ${CMAKE_MODULE_PATH}")
message(STATUS "Prefix Path: ${CMAKE_PREFIX_PATH}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(fineftp)

set(as_subproject fineftp)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" _cmp)
if (_cmp)
  macro(find_package)
    if(NOT "${ARGV0}" IN_LIST as_subproject)
      _find_package(${ARGV})
    endif()
  endmacro()
endif()

add_subdirectory(fineftp-server)
#add_subdirectory(example)
