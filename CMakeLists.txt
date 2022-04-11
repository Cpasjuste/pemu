cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# add libcross2d library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(libcross2d)

####################
# BUILD OPTIONS
####################
set(OPTION_EMU OFF CACHE STRING "Emulator to build (pfbneo, pnes, psnes)")
set(OPTION_BUILTIN_LIBCONFIG OFF CACHE BOOL "Build libconfig from sources")
set(OPTION_BUILTIN_MINIZIP OFF CACHE BOOL "Build minizip from sources")
set(OPTION_MPV_PLAYER ON CACHE BOOL "Add support for video snaps (mpv)")
set(OPTION_UPDATE_CONFIG ON CACHE BOOL "Force config file updating")
####################
# BUILD OPTIONS
####################

# linux/windows screen handling options
if (PLATFORM_LINUX)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif (PLATFORM_LINUX)

# add sscrap (screenscrapper scrapper!) library
add_subdirectory(sscrap)

# required for cmake toolchain setup
project(pemu)
set(PROJECT_AUTHOR cpasjuste)
set(VERSION_MAJOR 6)
set(VERSION_MINOR 0)

# pemu (ui)
add_subdirectory(ui)

# pnes
if (OPTION_EMU STREQUAL "pnes")
    add_subdirectory(pnes)
elseif (OPTION_EMU STREQUAL "psnes")
    # psnes
    add_subdirectory(psnes)
elseif (OPTION_EMU STREQUAL "pgen")
    # pgen
    add_subdirectory(pgen)
else ()
    # pfbneo
    add_subdirectory(pfbneo)
endif ()
