cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing a build setting/option
set(BUILD_LINUX ON CACHE BOOL "Build with Linux support")
set(BUILD_WINDOWS OFF CACHE BOOL "Build with Windows support")
set(BUILD_SWITCH OFF CACHE BOOL "Build with Nintendo Switch support")
set(BUILD_VITA OFF CACHE BOOL "Build with PSP2 support")
set(BUILD_3DS OFF CACHE BOOL "Build with 3DS support")

if (BUILD_WINDOWS)
    set(BUILD_LINUX ON)
endif(BUILD_WINDOWS)

if (BUILD_LINUX)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif (BUILD_LINUX)

set(UPDATE_CONFIG ON CACHE BOOL "Force config file updating")

# add ssscrap (screenscrapper scrapper!) library
add_subdirectory(ssscrap)

# add libcross2d/libcross2dui library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(libcross2dui)

# required for cmake toolchain setup
project(pemu)

set(PROJECT_AUTHOR Cpasjuste)
set(VERSION_MAJOR 3)
set(VERSION_MINOR 4)

# pnes
add_subdirectory(pnes)

# psnes
#add_subdirectory(psnes)

# pfba
#add_subdirectory(pfba)
