cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pgen)

set(OPTION_CHD ON CACHE STRING "build with CHD support")

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

set(GENESIS_CFLAGS
        -DUSE_16BPP_RENDERING
        -DHAVE_YM3438_CORE -DHAVE_OPLL_CORE
        -DLSB_FIRST -DMAXROMSIZE=33554432
        -D__PGEN__ -D__PGEN_VERSION_MAJOR__=${VERSION_MAJOR} -D__PGEN_VERSION_MINOR__=${VERSION_MINOR}
        )

if (OPTION_CHD)
    list(APPEND GENESIS_INC
            ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/libchdr/src
            ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/libchdr/deps/libFLAC/include
            ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/libchdr/deps/lzma
            )
    file(GLOB CHD_SRC ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/libchdr/src/*.c)
    file(GLOB FLAC_SRC ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/libchdr/deps/libFLAC/*.c)
    file(GLOB LZMA_SRC ${CMAKE_SOURCE_DIR}/cores/genesis/core/cd_hw/libchdr/deps/lzma/*.c)
    list(APPEND GENESIS_SRC ${CHD_SRC} ${FLAC_SRC} ${LZMA_SRC})
    list(APPEND GENESIS_CFLAGS
            -DUSE_LIBCHDR -DPACKAGE_VERSION=\"1.3.2\"
            -DFLAC_API_EXPORTS -DFLAC__HAS_OGG=0
            -DHAVE_LROUND -DHAVE_STDINT_H -D_7ZIP_ST)
endif ()

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
    list(APPEND GENESIS_CFLAGS -DHAVE_SYS_PARAM_H)
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND PGEN_SRC)
    list(APPEND PGEN_CFLAGS)
elseif (PLATFORM_3DS)
elseif (PLATFORM_LINUX OR PLATFORM_WINDOWS)
    #####################
    # SDL2 PLATORM
    #####################
    list(APPEND PGEN_CFLAGS)
elseif (PLATFORM_ANDROID)
    #####################
    # ANDROID PLATORM
    #####################
    if (ANDROID_ABI STREQUAL armeabi-v7a)
        list(APPEND PGEN_CFLAGS -marm -mcpu=cortex-a7 -mfpu=neon
                -ffunction-sections -fomit-frame-pointer) # fix crash with release optimizations
    endif ()
    list(APPEND GENESIS_CFLAGS -DHAVE_SYS_PARAM_H)
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

# add common skin data
add_custom_target(${PROJECT_NAME}.data.skin
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/common/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/${TARGET_PLATFORM}/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/common/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/${TARGET_PLATFORM}/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_custom.cmake
        )
add_dependencies(${PROJECT_NAME}.data.skin ${PROJECT_NAME}.data)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.skin)
