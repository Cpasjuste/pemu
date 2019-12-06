cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

####################
# TARGET PLATFORM
####################
set(PLATFORM_LINUX OFF CACHE BOOL "Linux platform target")
set(PLATFORM_WINDOWS OFF CACHE BOOL "Windows platform target (msys/mingw64)")
set(PLATFORM_RPI3 OFF CACHE BOOL "RIP3 platform target")
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
set(OPTION_SDL1 OFF CACHE BOOL "SDL1 support (software, slow, unsupported)")
set(OPTION_SDL2_GL OFF CACHE BOOL "SDL2 support (OpenGL 3.3)")
set(OPTION_SDL2_GLES2 OFF CACHE BOOL "SDL2 support (OpenGLES 2.0)")
set(OPTION_SDL2_SOFT OFF CACHE BOOL "SDL2 support (software renderer)")
set(OPTION_BUILTIN_LIBCONFIG OFF CACHE BOOL "Build libconfig from sources")
set(OPTION_BUILTIN_MINIZIP OFF CACHE BOOL "Build libconfig from sources")
set(OPTION_FREEPLAY OFF CACHE BOOL "Build with FreePlay (CM3/ZERO) support")
set(OPTION_RECALBOX_BUILDROOT OFF CACHE BOOL "Build from recalbox buildroot")
set(OPTION_SCREEN_WIDTH 1280 CACHE STRING "Custom screen width")
set(OPTION_SCREEN_HEIGHT 720 CACHE STRING "Custom screen height")
####################
# BUILD OPTIONS
####################

####################
# PEMU OPTIONS
####################
set(MPV_PLAYER ON CACHE BOOL "Add support for video snaps (mpv)")
set(UPDATE_CONFIG ON CACHE BOOL "Force config file updating")

# add libcross2d library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
add_subdirectory(libcross2d)

if (MPV_PLAYER)
	if(PLATFORM_SWITCH)
		add_subdirectory(mpv)
	endif ()
endif ()

# add sscrap (screenscrapper scrapper!) library
add_subdirectory(sscrap)

# add ui library
if (PLATFORM_LINUX)
    set(OPTION_RESOLUTION ON CACHE BOOL "Add screen resolution option")
    set(OPTION_FULLSCREEN ON CACHE BOOL "Add switch to fullscreen option")
endif (PLATFORM_LINUX)
add_subdirectory(ui)

# required for cmake toolchain setup
project(pemu)

set(PROJECT_AUTHOR Cpasjuste)
set(VERSION_MAJOR 4)
set(VERSION_MINOR 0)

# pnes
#add_subdirectory(pnes)

# psnes
#add_subdirectory(psnes)

# pfba
add_subdirectory(pfba)
