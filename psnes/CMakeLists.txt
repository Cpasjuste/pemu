cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(psnes)

##############
# SNES9X
##############
set(OPTION_SOFT_SCALERS OFF CACHE BOOL "Build with software scalers support")
set(OPTION_SOUND ON CACHE BOOL "Build with audio support")
set(OPTION_SCREENSHOT ON CACHE BOOL "Build with screenshot support")
set(OPTION_NETPLAY OFF CACHE BOOL "Build with screenshot support")
set(OPTION_GAMEPAD ON CACHE BOOL "Build with gamepad support")
set(OPTION_GZIP ON CACHE BOOL "Build with gzip support")
set(OPTION_ZIP ON CACHE BOOL "Build with zip support")
set(OPTION_JMA OFF CACHE BOOL "Build with jma support")
set(OPTION_DEBUGGER OFF CACHE BOOL "Build with debugger support")

set(SNES9X_INC
        ${CMAKE_SOURCE_DIR}/cores/snes9x
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu
        ${CMAKE_SOURCE_DIR}/cores/snes9x/filter
        )

file(GLOB SNES9X_SRC
        ${CMAKE_SOURCE_DIR}/cores/snes9x/*.c*
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/*.c*
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/*.c*
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/dsp/*.c*
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/smp/*.c*
        ${CMAKE_SOURCE_DIR}/cores/snes9x/filter/*.c*
        )

list(REMOVE_ITEM SNES9X_SRC
        ${CMAKE_SOURCE_DIR}/cores/snes9x/spc7110dec.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/spc7110emu.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/srtcemu.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/smp/algorithms.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/smp/core.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/smp/memory.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/smp/timing.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/smp/iplrom.cpp
        ${CMAKE_SOURCE_DIR}/cores/snes9x/apu/bapu/dsp/SPC_DSP.cpp
        )

set(SNES9X_CFLAGS
        -fomit-frame-pointer -fno-exceptions
        -pedantic -Wall -W -Wno-unused-parameter
        -DHAVE_MKSTEMP -DHAVE_STRINGS_H
        -DHAVE_SYS_IOCTL_H -DHAVE_STDINT_H -DRIGHTSHIFT_IS_SAR
        -DGFX_MULTI_FORMAT
        -D__BYTE_ORDER__=__ORDER_LITTLE_ENDIAN__
        -D__PSNES__
        -D__PSNES_VERSION_MAJOR__=${VERSION_MAJOR}
        -D__PSNES_VERSION_MINOR__=${VERSION_MINOR}
        )

set(SNES9X_LDFLAGS m z)

#####################
# OPTIONS
#####################
if (OPTION_SOFT_SCALERS)
    list(APPEND SNES9X_CFLAGS -D__SOFT_SCALERS__)
endif (OPTION_SOFT_SCALERS)
if (OPTION_SOUND)
else ()
    list(APPEND SNES9X_CFLAGS -DNOSOUND)
endif (OPTION_SOUND)
if (OPTION_SCREENSHOT)
endif (OPTION_SCREENSHOT)
if (OPTION_NETPLAY)
endif (OPTION_NETPLAY)
if (OPTION_GAMEPAD)
    list(APPEND SNES9X_CFLAGS -DJOYSTICK_SUPPORT)
endif (OPTION_GAMEPAD)
if (OPTION_GZIP)
    list(APPEND SNES9X_CFLAGS -DZLIB)
endif (OPTION_GZIP)
if (OPTION_ZIP)
    # use libminizip
    if (NOT OPTION_BUILTIN_MINIZIP)
        list(APPEND SNES9X_LDFLAGS minizip)
    endif ()
    list(APPEND SNES9X_CFLAGS -DUNZIP_SUPPORT -DSYSTEM_ZIP)
endif (OPTION_ZIP)
if (OPTION_JMA)
    set(JMA_INC ${CMAKE_SOURCE_DIR}/cores/snes9x/jma)
    file(GLOB JMA_SRC ${CMAKE_SOURCE_DIR}/cores/snes9x/jma/*.c*)
    list(APPEND SNES9X_CFLAGS -DJMA_SUPPORT)
endif (OPTION_JMA)
if (OPTION_DEBUGGER)
endif (OPTION_DEBUGGER)

##############
# psnes
##############
set(PSNES_INC sources)
file(GLOB PSNES_SRC sources/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # PSP2 PLATORM
    #####################
    set(TITLE_ID "PSNE00001" CACHE STRING "" FORCE)
    list(APPEND SNES9X_LDFLAGS ScePower_stub)
elseif (PLATFORM_SWITCH)
    #####################
    # NX PLATORM
    #####################
    list(APPEND SNES9X_CFLAGS -Wno-pedantic -Wno-missing-field-initializers -Wno-format-truncation)
endif ()

add_executable(${PROJECT_NAME} ${PSNES_SRC} ${SNES9X_SRC} ${UNZIP_SRC} ${JMA_SRC})
target_compile_options(${PROJECT_NAME} PRIVATE ${SNES9X_CFLAGS})
target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++11 -fpermissive>)
target_include_directories(${PROJECT_NAME} PRIVATE ${PSNES_INC} ${SNES9X_INC} ${UNZIP_INC} ${JMA_INC})
target_link_libraries(${PROJECT_NAME} cross2dui ${SNES9X_LDFLAGS})

# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)

# add extra data to libcross2d default targets data (zipped skin)
add_custom_target(${PROJECT_NAME}.data.extra
        DEPENDS ${PROJECT_NAME}.data
        # make dummy directory for user files
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/roms
        # compress skin
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default.zip .
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        # remove non-zipped skin from data directory (used for program execution from cmake build) before zipping it for release
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default
        # compress default_320x240 skin
        #COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240 && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240.zip .
        #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        # remove non-zipped skin from data directory (used for program execution from cmake build) before zipping it for release
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra)
add_dependencies(${PROJECT_NAME}_${TARGET_PLATFORM}_release ${PROJECT_NAME}.data.extra)

if (PLATFORM_WINDOWS)
    add_custom_command(TARGET ${PROJECT_NAME}.data.extra POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/pfba/data/windows/copy_deps.sh ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMENT "Copying required mingw dependencies..."
            )
endif ()
