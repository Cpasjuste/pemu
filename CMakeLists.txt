cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing a build setting/option
set(BUILD_LINUX ON CACHE BOOL "Build with Linux support")
set(BUILD_WINDOWS OFF CACHE BOOL "Build with Windows support")
set(BUILD_VITA OFF CACHE BOOL "Build with PSP2 support")
set(BUILD_3DS OFF CACHE BOOL "Build with 3DS support")
set(BUILD_SWITCH OFF CACHE BOOL "Build with Nintendo Switch support")

set(UPDATE_CONFIG OFF CACHE BOOL "Force config file updating")

if (BUILD_LINUX)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif (BUILD_LINUX)

# add libcross2d/libcross2dui library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(libcross2dui)

# required for cmake toolchain setup
project(pemu)

# pnes
set(PNES_VERSION_MAJOR 3)
set(PNES_VERSION_MINOR 0)
add_subdirectory(pnes)

# psnes
set(PSNES_VERSION_MAJOR 3)
set(PSNES_VERSION_MINOR 0)
add_subdirectory(psnes)

# pfba
set(PFBA_VERSION_MAJOR 3)
set(PFBA_VERSION_MINOR 0)
add_subdirectory(pfba)
