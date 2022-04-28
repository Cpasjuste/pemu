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
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pSNES - Portable Snes9x")
    set(PS4_PKG_TITLE_ID "PSNE00001")
    set(PS4_PKG_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
elseif (PLATFORM_SWITCH)
    #####################
    # NX PLATORM
    #####################
    list(APPEND SNES9X_CFLAGS -Wno-pedantic -Wno-missing-field-initializers -Wno-format-truncation)
endif ()

add_executable(${PROJECT_NAME} ${PSNES_SRC} ${SNES9X_SRC} ${UNZIP_SRC} ${JMA_SRC})
target_compile_options(${PROJECT_NAME} PRIVATE ${SNES9X_CFLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${PSNES_INC} ${SNES9X_INC} ${UNZIP_INC} ${JMA_INC})
target_link_libraries(${PROJECT_NAME} cross2dui ${SNES9X_LDFLAGS})

# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)

# add common skin data
add_custom_target(${PROJECT_NAME}.data.skin
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/common/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/${TARGET_PLATFORM}/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_directory_custom.cmake
        )
add_dependencies(${PROJECT_NAME}.data.skin ${PROJECT_NAME}.data)
if (PLATFORM_LINUX)
    add_dependencies(${PROJECT_NAME}-romfs ${PROJECT_NAME}.data.skin)
else ()
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.skin)
endif ()

if (PLATFORM_WINDOWS)
    add_custom_command(TARGET ${PROJECT_NAME}.data POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/pfba/data/windows/copy_deps.sh ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMENT "Copying required mingw dependencies..."
            )
endif ()
