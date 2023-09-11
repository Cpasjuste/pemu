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
set(OPTION_UPDATE_CONFIG OFF CACHE BOOL "Force config file updating")
set(OPTION_FTP_SERVER OFF CACHE BOOL "Embedded ftp server")
####################
# BUILD OPTIONS
####################

# linux/windows screen handling options
if (PLATFORM_LINUX OR PLATFORM_WINDOWS)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif ()

# add sscrap (screenscrapper scrapper!) library
add_subdirectory(sscrap)

# required for cmake toolchain setup
project(pemu)
set(PROJECT_AUTHOR cpasjuste)
set(VERSION_MAJOR 6)
set(VERSION_MINOR 7)

# pemu (ui)
add_subdirectory(ui)

# emulator selection
if (OPTION_EMU STREQUAL "pnes")
    # pnes
    add_subdirectory(pnes)
elseif (OPTION_EMU STREQUAL "psnes")
    # psnes
    add_subdirectory(psnes)
elseif (OPTION_EMU STREQUAL "pgen")
    # pgen
    add_subdirectory(pgen)
elseif (OPTION_EMU STREQUAL "pfbneo")
    # pfbneo
    add_subdirectory(pfbneo)
else ()
    add_subdirectory(pfbneo)
    add_subdirectory(pgen)
    add_subdirectory(pnes)
    add_subdirectory(psnes)
endif ()
