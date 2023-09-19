cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# add libcross2d library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(external/libcross2d)

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
add_subdirectory(external/sscrap)

# required for cmake toolchain setup
project(pemu)
set(PROJECT_AUTHOR cpasjuste)
set(VERSION_MAJOR 6)
set(VERSION_MINOR 7)

# pemu (ui)
add_subdirectory(src/skeleton)

# emulator selection
if (OPTION_EMU STREQUAL "src/cores/pnes")
    # pnes
    add_subdirectory(src/cores/pnes)
elseif (OPTION_EMU STREQUAL "src/cores/psnes")
    # psnes
    add_subdirectory(src/cores/psnes)
elseif (OPTION_EMU STREQUAL "src/cores/pgen")
    # pgen
    add_subdirectory(src/cores/pgen)
elseif (OPTION_EMU STREQUAL "src/cores/pfbneo")
    # pfbneo
    add_subdirectory(src/cores/pfbneo)
else ()
    add_subdirectory(src/cores/pfbneo)
    add_subdirectory(src/cores/pgen)
    add_subdirectory(src/cores/pnes)
    add_subdirectory(src/cores/psnes)
endif ()
