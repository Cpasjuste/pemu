cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
set(PLATFORM_LINUX OFF CACHE BOOL "Linux platform target")
set(PLATFORM_WINDOWS OFF CACHE BOOL "Windows platform target (msys/mingw64)")
set(PLATFORM_RPI3_64 OFF CACHE BOOL "RIP3 (64 bits) platform target")
set(PLATFORM_SWITCH OFF CACHE BOOL "Nintendo Switch platform target")
set(PLATFORM_3DS OFF CACHE BOOL "Nintendo 3DS platform target")
set(PLATFORM_VITA OFF CACHE BOOL "Sony PS Vita platform target")
set(PLATFORM_LDK OFF CACHE BOOL "LDK Handheld platform target (mips: LDK, RS-97...)")
####################
# TARGET PLATFORM
####################

####################
# BUILD OPTIONS
####################
set(OPTION_SDL2_GL OFF CACHE BOOL "SDL2 support (OpenGL 3.3)")
set(OPTION_SDL2_GLES2 OFF CACHE BOOL "SDL2 support (OpenGLES 2.0)")
set(OPTION_SDL2_SOFT OFF CACHE BOOL "SDL2 support (software renderer)")
set(OPTION_BUILTIN_LIBCONFIG OFF CACHE BOOL "Build libconfig from sources")
set(OPTION_RECALBOX OFF CACHE BOOL "Build from recalbox buildroot")
####################
# BUILD OPTIONS
####################

#include(libcross2dui/libcross2d/Toolchain.cmake)

# add libcross2d/libcross2dui library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(libcross2dui)

set(OPTION_SSCRAP OFF CACHE BOOL "Build with screenscrapper support")
set(UPDATE_CONFIG ON CACHE BOOL "Force config file updating")
if (PLATFORM_LINUX)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif (PLATFORM_LINUX)
if (OPTION_SSCRAP)
    # add ssscrap (screenscrapper scrapper!) library
    add_subdirectory(ssscrap)
endif ()

# required for cmake toolchain setup
project(pemu)

set(PROJECT_AUTHOR Cpasjuste)
set(VERSION_MAJOR 3)
set(VERSION_MINOR 4)

# pnes
#add_subdirectory(pnes)

# psnes
#add_subdirectory(psnes)

# pfba
add_subdirectory(pfba)
