cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing a build setting/option
set(BUILD_SFML OFF CACHE BOOL "Build with SFML support")
set(BUILD_SDL1 OFF CACHE BOOL "Build with SDL1 support (software, osmesa) support")
set(BUILD_SDL1_GL OFF CACHE BOOL "Build with SDL1 support (hardware rendering, fast, needs BUILD_SDL1=ON)")
set(BUILD_SDL2 ON CACHE BOOL "Build with SDL2 (software) support")
set(BUILD_SDL2_GL ON CACHE BOOL "Build with SDL2 support (hardware rendering, fast, needs BUILD_SDL2=ON)")
set(BUILD_PSP2 OFF CACHE BOOL "Build with PSP2 support")
set(BUILD_PS3 OFF CACHE BOOL "Build with PS3 support")
set(BUILD_3DS OFF CACHE BOOL "Build with 3DS support")
set(BUILD_NX OFF CACHE BOOL "Build with NX (SWITCH) support")

set(BUILD_PFBA OFF CACHE BOOL "Build pFBA (fba)")
set(BUILD_PSNES OFF CACHE BOOL "Build pSNES (snes9x)")
set(BUILD_PNES ON CACHE BOOL "Build pNES (nestopia)")

set(UPDATE_CONFIG OFF CACHE BOOL "Force config file updating")

if (BUILD_SDL2)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif (BUILD_SDL2)

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
