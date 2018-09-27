cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

# delete cmake cache folder before changing this options
option(BUILD_SFML "Build with SFML support (hardware rendering, fast)" OFF)
option(BUILD_SDL1 "Build with SDL1 support (software, osmesa) support" OFF)
option(BUILD_SDL1_GL "Build with SDL1 support (hardware rendering, fast, needs BUILD_SDL1=ON)" OFF)
option(BUILD_SDL2 "Build with SDL2 support (software)" OFF)
option(BUILD_SDL2_GL "Build with SDL2 support (hardware rendering, fast, needs BUILD_SDL2=ON)" OFF)
option(BUILD_PSP2 "Build with PSP2 support (hardware rendering, fast)" OFF)
option(BUILD_PS3 "Build with PS3 support (hardware rendering, fast)" OFF)
option(BUILD_3DS "Build with 3DS support" OFF)
option(BUILD_NX "Build with NX (SWITCH) support" OFF)

option(BUILD_PFBA "Build pFBA (fba)" OFF)
option(BUILD_PSNES "Build pSNES (snes9x)" OFF)
option(BUILD_PNES "Build pNES (nestopia)" OFF)

option(UPDATE_CONFIG "Force config file updating" OFF)

execute_process(COMMAND "date" "+%Y.%m.%d" OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)

# add libcross2d/libcross2dui library. If you want to port pfba to another platform,
# you should (may) only need to port libcross2d library.
#include(${CMAKE_SOURCE_DIR}/libcross2dui/libcross2d/Toolchain.cmake)
#add_subdirectory(libcross2dui)

project(pnes)

##############
# NESTOPIA
##############
set(NESTOPIA_INC
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/api
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/board
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/input
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/vssystem
        )

file(GLOB NESTOPIA_SRC
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/*.c*
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/api/*.c*
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/board/*.c*
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/input/*.c*
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/vssystem/*.c*
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/*.c*
        )

list(REMOVE_ITEM NESTOPIA_SRC
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/video.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/config.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilter2xSaI.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterHqX.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterScaleX.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterxBR
        )

set(NESTOPIA_CFLAGS
        -DVERSION="1.0"
        -D__LIBRETRO__ -D_MINGW -D__NST_UNZIP__
        -D__PNES__
        -D__PNES_VERSION_MAJOR__=${PNES_VERSION_MAJOR}
        -D__PNES_VERSION_MINOR__=${PNES_VERSION_MINOR}
        -Wno-unused-definition -Wno-unused-local-typedefs
        -Wno-sign-compare -Wno-unused-variable
        -Wno-delete-non-virtual-dtor -Wno-reorder
        -Wno-parentheses -Wno-unknown-pragmas
        -Wno-format-truncation -Wno-unused-but-set-variable
        -Wno-format-zero-length
        )

set(NESTOPIA_LDFLAGS minizip)

if (BUILD_DEBUG)
    list(APPEND NESTOPIA_CFLAGS -g -O0)
endif (BUILD_DEBUG)

##############
# pnes
##############
set(PNES_INC pnes)
file(GLOB PNES_SRC pnes/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (BUILD_PSP2)
    #####################
    # PSP2 PLATORM
    #####################
    list(APPEND NESTOPIA_LDFLAGS ScePower_stub)
elseif (BUILD_NX)
    #####################
    # NX PLATORM
    #####################
    list(APPEND NESTOPIA_SRC pnes/switch/port.c)
elseif (BUILD_SDL2)
    #####################
    # SDL2 PLATORM
    #####################
endif (BUILD_PSP2)

#####################
# pnes executable
#####################
add_custom_target(${PROJECT_NAME}.data
        COMMAND cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND cp -f ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME}.elf ${PNES_SRC} ${NESTOPIA_SRC})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${PNES_INC} ${NESTOPIA_INC})
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${NESTOPIA_CFLAGS})
target_compile_options(${PROJECT_NAME}.elf PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++14 -fexceptions>)
target_link_libraries(${PROJECT_NAME}.elf cross2dui ${NESTOPIA_LDFLAGS})
add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME}.data)

#####################
# PSP2 (vita) target
#####################
if (BUILD_PSP2)
    vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME}.elf)
    add_custom_target(${PROJECT_NAME}_vita_release.zip
            DEPENDS ${PROJECT_NAME}.self
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/sce_sys
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="PNES0001" "pnes" ${CMAKE_BINARY_DIR}/release/pnes/sce_sys/param.sfo
            COMMAND cp -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.self ${CMAKE_BINARY_DIR}/release/pnes/eboot.bin
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin ${CMAKE_BINARY_DIR}/release/pnes/
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/psp2/skin ${CMAKE_BINARY_DIR}/release/pnes/
            COMMAND cd ${CMAKE_BINARY_DIR}/release/pnes && zip -r ../../${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_vita.vpk . && cd ${CMAKE_BINARY_DIR}
            )
endif (BUILD_PSP2)

#####################
# NX (SWITCH) target
#####################
if (BUILD_NX)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/libnx/switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND nacptool --create "pNES" "Cpasjuste" "${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}" pnes.nacp
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/nx/icon.jpg --nacp=pnes.nacp)
    add_custom_target(${PROJECT_NAME}_switch_release.zip
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/roms
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/titles
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/previews
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/icons
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/pnes.nro ${CMAKE_BINARY_DIR}/release/pnes/pnes.nro
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin ${CMAKE_BINARY_DIR}/release/pnes/
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/nx/skin ${CMAKE_BINARY_DIR}/release/pnes/
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_switch.zip pnes
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}
            )
endif (BUILD_NX)
