cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pgen)

##################
# GENESIS PLUS GX
##################
set(GENESIS_INC
        ${CMAKE_SOURCE_DIR}/cores/genesis/core
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/cart_hw
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/cart_hw/svp
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/debug
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/input_hw
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/m68k
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/ntsc
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/sound
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/z80
        )

file(GLOB GENESIS_SRC
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/cart_hw/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/cart_hw/svp/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/*.c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/debug/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/input_hw/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/m68k/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/ntsc/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/sound/*c*
        ${CMAKE_SOURCE_DIR}/cores/genesis/core/z80/*c*
        )

list(REMOVE_ITEM GENESIS_SRC
        )

set(GENESIS_CFLAGS
        -DUSE_16BPP_RENDERING -DHAVE_YM3438_CORE -DHAVE_OPLL_CORE -DLSB_FIRST
        -DVERSION="1.0"
        -D__PGEN__ -D__PGEN_VERSION_MAJOR__=${VERSION_MAJOR} -D__PGEN_VERSION_MINOR__=${VERSION_MINOR}
        )

##############
# pgen
##############
set(PGEN_INC sources)
file(GLOB PGEN_SRC sources/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # PS VITA PLATORM
    #####################
    set(TITLE_ID "PGEN00001" CACHE STRING "" FORCE)
    list(APPEND PGEN_INC)
    list(APPEND PGEN_SRC)
    list(APPEND PGEN_CFLAGS)
    list(APPEND PGEN_LDFLAGS)
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pGEN - Portable Genesis Plus GX")
    set(PS4_PKG_TITLE_ID "PGEN00001")
    set(PS4_PKG_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND PGEN_SRC)
    list(APPEND PGEN_CFLAGS)
elseif (PLATFORM_3DS)
elseif (PLATFORM_LINUX)
    #####################
    # SDL2 PLATORM
    #####################
    list(APPEND PGEN_CFLAGS)
endif (PLATFORM_VITA)

#####################
# pgen executable
#####################
# add main executable (elf)
add_executable(${PROJECT_NAME} ${PGEN_SRC} ${GENESIS_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PGEN_INC} ${GENESIS_INC})
target_compile_options(${PROJECT_NAME} PRIVATE ${GENESIS_CFLAGS} ${PGEN_CFLAGS})
target_link_libraries(${PROJECT_NAME} cross2dui ${GENESIS_LDFLAGS} ${PGEN_LDFLAGS})

# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)