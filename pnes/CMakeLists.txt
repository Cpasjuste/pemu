cmake_minimum_required(VERSION 3.2)
#set(CMAKE_VERBOSE_MAKEFILE ON)

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
if (BUILD_VITA)
    #####################
    # PS VITA PLATORM
    #####################
    list(APPEND NESTOPIA_LDFLAGS ScePower_stub)
elseif (BUILD_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND NESTOPIA_SRC pnes/switch/port.c)
elseif (BUILD_LINUX)
    #####################
    # SDL2 PLATORM
    #####################
	if(BUILD_WINDOWS)
		#list(APPEND NESTOPIA_CFLAGS -D_MINGW)
	endif(BUILD_WINDOWS)
endif (BUILD_VITA)

#####################
# pnes executable
#####################
message(FATAL_ERROR "TODO")
add_custom_target(${PROJECT_NAME}.data
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/skins
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/skins
        COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_CURRENT_BINARY_DIR}/skins/default.zip --format=zip -- .
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin)
        #COMMAND zip -j ${CMAKE_CURRENT_BINARY_DIR}/skins/default.zip ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin/*
        #COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/nx/skin/ && zip -ru ${CMAKE_CURRENT_BINARY_DIR}/skins/default.zip .
        #COMMAND cp -f ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJECT_NAME}.elf ${PNES_SRC} ${NESTOPIA_SRC})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${PNES_INC} ${NESTOPIA_INC})
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${NESTOPIA_CFLAGS})
target_compile_options(${PROJECT_NAME}.elf PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++14 -fexceptions>)
target_link_libraries(${PROJECT_NAME}.elf cross2dui ${NESTOPIA_LDFLAGS})
add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME}.data)

###########################
# LINUX (ubuntu, release)
###########################
if (BUILD_LINUX)
    add_custom_target(${PROJECT_NAME}_linux_release.zip
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/skins
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/roms
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/titles
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/previews
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/icons
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/pnes.elf ${CMAKE_BINARY_DIR}/release/pnes/pnes
            COMMAND cp -f ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_BINARY_DIR}/release/pnes/
            # zip skin
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes/skins/default.zip
            COMMAND zip -j ${CMAKE_BINARY_DIR}/release/pnes/skins/default.zip ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin/*
            # zip release
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_linux.zip
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_linux.zip pnes
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}
            )
endif (BUILD_LINUX)

#####################
# PSP2 (vita) target
#####################
if (BUILD_VITA)
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
endif ()

#####################
# SWITCH target
#####################
if (BUILD_SWITCH)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/libnx/switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND nacptool --create "pNES" "Cpasjuste" "${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}" pnes.nacp
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/nx/icon.jpg --nacp=pnes.nacp)
    add_custom_target(${PROJECT_NAME}_switch_release.zip
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/skins
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/roms
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/titles
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/previews
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pnes/icons
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/pnes.nro ${CMAKE_BINARY_DIR}/release/pnes/pnes.nro
            COMMAND cp -f ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_BINARY_DIR}/release/pnes/
            # zip skin
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pnes/skins/default.zip
            COMMAND zip -j ${CMAKE_BINARY_DIR}/release/pnes/skins/default.zip ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/skin/*
            COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/pnes/data/nx/skin/ && zip -ru ${CMAKE_BINARY_DIR}/release/pnes/skins/default.zip .
            # zip release
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_switch.zip
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${PNES_VERSION_MAJOR}.${PNES_VERSION_MINOR}_switch.zip pnes
            COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}
            )
endif ()
