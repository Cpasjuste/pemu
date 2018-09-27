cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing this option
set(BUILD_SFML OFF CACHE BOOL "Build with SFML support")
set(BUILD_SDL1 OFF CACHE BOOL "Build with SDL1 support (software, osmesa) support")
set(BUILD_SDL1_GL OFF CACHE BOOL "Build with SDL1 support (hardware rendering, fast, needs BUILD_SDL1=ON)")
set(BUILD_SDL2 OFF CACHE BOOL "Build with SDL2 (software) support")
set(BUILD_SDL2_GL OFF CACHE BOOL "Build with SDL2 support (hardware rendering, fast, needs BUILD_SDL2=ON)")
set(BUILD_PSP2 OFF CACHE BOOL "Build with PSP2 support")
set(BUILD_PS3 OFF CACHE BOOL "Build with PS3 support")
set(BUILD_3DS OFF CACHE BOOL "Build with 3DS support")
set(BUILD_NX ON CACHE BOOL "Build with NX (SWITCH) support")

set(BUILD_PFBA ON CACHE BOOL "Build pFBA (fba)")
set(BUILD_PSNES OFF CACHE BOOL "Build pSNES (snes9x)")
set(BUILD_PNES OFF CACHE BOOL "Build pNES (nestopia)")

set(UPDATE_CONFIG OFF CACHE BOOL "Force config file updating")

# add libcross2d/libcross2dui library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(libcross2dui)
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

# required for cmake toolchain setup
project(pemu)

if (BUILD_PNES)
    set(PNES_VERSION_MAJOR 1)
    set(PNES_VERSION_MINOR 0)
    add_subdirectory(pnes)
endif(BUILD_PNES)

if (BUILD_PSNES)
    set(PSNES_VERSION_MAJOR 2)
    set(PSNES_VERSION_MINOR 2)
    add_subdirectory(psnes)
endif(BUILD_PSNES)

if (BUILD_PFBA)
    set(PFBA_VERSION_MAJOR 2)
    set(PFBA_VERSION_MINOR 0)
    add_subdirectory(pfba)
endif(BUILD_PFBA)
