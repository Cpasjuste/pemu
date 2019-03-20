cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(psnes)

##############
# SNES9X
##############
set(BUILD_SOFT_SCALERS OFF CACHE BOOL "Build with software scalers support")
set(BUILD_SOUND ON CACHE BOOL "Build with audio support")
set(BUILD_SCREENSHOT ON CACHE BOOL "Build with screenshot support")
set(BUILD_NETPLAY OFF CACHE BOOL "Build with screenshot support")
set(BUILD_GAMEPAD ON CACHE BOOL "Build with gamepad support")
set(BUILD_GZIP ON CACHE BOOL "Build with gzip support")
set(BUILD_ZIP ON CACHE BOOL "Build with zip support")
set(BUILD_JMA OFF CACHE BOOL "Build with jma support")
set(BUILD_DEBUGGER OFF CACHE BOOL "Build with debugger support")

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
        ${CMAKE_SOURCE_DIR}/cores/snes9x/filter/blit.cpp
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
        )

set(SNES9X_CFLAGS
        -fomit-frame-pointer -fno-exceptions
        -pedantic -Wall -W -Wno-unused-parameter
        -DHAVE_MKSTEMP -DHAVE_STRINGS_H
        -DHAVE_SYS_IOCTL_H -DHAVE_STDINT_H -DRIGHTSHIFT_IS_SAR
        -DGFX_MULTI_FORMAT
        -D__BYTE_ORDER__=__ORDER_LITTLE_ENDIAN__
        -D__PSNES__
        -D__PSNES_VERSION_MAJOR__=${PSNES_VERSION_MAJOR}
        -D__PSNES_VERSION_MINOR__=${PSNES_VERSION_MINOR}
        )

if (BUILD_DEBUG)
    list(APPEND SNES9X_CFLAGS -g -O0)
endif (BUILD_DEBUG)

set(SNES9X_LDFLAGS m z)

#####################
# OPTIONS
#####################
if (BUILD_SOFT_SCALERS)
    file(GLOB SNES9X_SCALERS_SRC ${CMAKE_SOURCE_DIR}/cores/snes9x/filter/*.c*)
    list(APPEND SNES9X_SRC ${SNES9X_SCALERS_SRC})
    list(APPEND SNES9X_CFLAGS -D__SOFT_SCALERS__)
endif (BUILD_SOFT_SCALERS)
if (BUILD_SOUND)
else ()
    list(APPEND SNES9X_CFLAGS -DNOSOUND)
endif (BUILD_SOUND)
if (BUILD_SCREENSHOT)
endif (BUILD_SCREENSHOT)
if (BUILD_NETPLAY)
endif (BUILD_NETPLAY)
if (BUILD_GAMEPAD)
    list(APPEND SNES9X_CFLAGS -DJOYSTICK_SUPPORT)
endif (BUILD_GAMEPAD)
if (BUILD_GZIP)
    list(APPEND SNES9X_CFLAGS -DZLIB)
endif (BUILD_GZIP)
if (BUILD_ZIP)
    # use libminizip
    list(APPEND SNES9X_LDFLAGS minizip)
    list(APPEND SNES9X_CFLAGS -DUNZIP_SUPPORT -DSYSTEM_ZIP)
endif (BUILD_ZIP)
if (BUILD_JMA)
    set(JMA_INC ${CMAKE_SOURCE_DIR}/cores/snes9x/jma)
    file(GLOB JMA_SRC ${CMAKE_SOURCE_DIR}/cores/snes9x/jma/*.c*)
    list(APPEND SNES9X_CFLAGS -DJMA_SUPPORT)
endif (BUILD_JMA)
if (BUILD_DEBUGGER)
endif (BUILD_DEBUGGER)

##############
# psnes
##############
set(PSNES_INC psnes)
file(GLOB PSNES_SRC psnes/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (BUILD_PSP2)
    #####################
    # PSP2 PLATORM
    #####################
    list(APPEND SNES9X_LDFLAGS ScePower_stub)
elseif (BUILD_SWITCH)
    #####################
    # NX PLATORM
    #####################
    list(APPEND SNES9X_CFLAGS -Wno-pedantic -Wno-missing-field-initializers -Wno-format-truncation)
elseif (BUILD_SDL2)
    #####################
    # SDL2 PLATORM
    #####################
endif (BUILD_PSP2)

#####################
# snes9x executable
#####################
add_custom_target(${PROJECT_NAME}.data
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/skins
        COMMAND zip -j ${CMAKE_CURRENT_BINARY_DIR}/skins/default.zip ${CMAKE_CURRENT_SOURCE_DIR}/psnes/data/skin/*
        COMMAND cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/psnes/data/databases/Super_Nintendo_Entertainment_System-hyperspin.xml ${CMAKE_CURRENT_BINARY_DIR}/db.xml
        )
add_executable(${PROJECT_NAME}.elf ${PSNES_SRC} ${SNES9X_SRC} ${UNZIP_SRC} ${JMA_SRC})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${PSNES_INC} ${SNES9X_INC} ${UNZIP_INC} ${JMA_INC})
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${SNES9X_CFLAGS})
target_compile_options(${PROJECT_NAME}.elf PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>)
target_link_libraries(${PROJECT_NAME}.elf cross2dui ${SNES9X_LDFLAGS})
add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME}.data)

#####################
# PSP2 (vita) target
#####################
if (BUILD_PSP2)
    vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME}.elf)
    add_custom_target(${PROJECT_NAME}_vita_release.zip
            DEPENDS ${PROJECT_NAME}.self
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/psnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes/sce_sys
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="PSNES0001" "pSNES" ${CMAKE_BINARY_DIR}/release/psnes/sce_sys/param.sfo
            COMMAND cp -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.self ${CMAKE_BINARY_DIR}/release/psnes/eboot.bin
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/psnes/data/skin ${CMAKE_BINARY_DIR}/release/psnes/
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/psnes/data/psp2/skin ${CMAKE_BINARY_DIR}/release/psnes/
            #COMMAND cp -f ${CMAKE_SOURCE_DIR}/psnes/data/databases/Super_Nintendo_Entertainment_System-hyperspin.xml ${CMAKE_BINARY_DIR}/release/psnes/db.xml
            COMMAND cd ${CMAKE_BINARY_DIR}/release/psnes && zip -r ../../${PROJECT_NAME}-${PSNES_VERSION_MAJOR}.${PSNES_VERSION_MINOR}_vita.vpk . && cd ${CMAKE_BINARY_DIR}
            )
endif (BUILD_PSP2)

#####################
# NX (SWITCH) target
#####################
if (BUILD_SWITCH)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/libnx/switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND nacptool --create "pSNES" "Cpasjuste" "${PSNES_VERSION_MAJOR}.${PSNES_VERSION_MINOR}" psnes.nacp
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/psnes/data/nx/icon.jpg --nacp=psnes.nacp)
    add_custom_target(${PROJECT_NAME}_switch_release.zip
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/psnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes/skins
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes/roms
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes/titles
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes/previews
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/psnes/icons
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/psnes.nro ${CMAKE_BINARY_DIR}/release/psnes/psnes.nro
            #TODO: buttons
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/psnes/data/nx/skin/* ${CMAKE_BINARY_DIR}/release/psnes/skins/
            COMMAND zip -j ${CMAKE_BINARY_DIR}/release/psnes/skins/default.zip ${CMAKE_CURRENT_SOURCE_DIR}/psnes/data/skin/*
            COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/psnes/data/databases/Super_Nintendo_Entertainment_System-hyperspin.xml ${CMAKE_BINARY_DIR}/release/psnes/db.xml
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${PSNES_VERSION_MAJOR}.${PSNES_VERSION_MINOR}_switch.zip psnes && cd ${CMAKE_BINARY_DIR}
            )
endif (BUILD_SWITCH)
