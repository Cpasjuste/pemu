cmake_minimum_required(VERSION 3.0)
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
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/fltkui/cheats.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/fltkui/homebrew.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/fltkui/samples.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/fltkui/nstcommon.cpp
        )
list(REMOVE_ITEM NESTOPIA_SRC
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilter2xSaI.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterHqX.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterScaleX.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterxBR.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterNtsc.cpp
        )

set(NESTOPIA_CFLAGS
        -DVERSION="1.51.1+"
        -D_MINGW # for nst_sample_load_archive
        -DNST_NO_2XSAI -DNST_NO_HQ2X -DNST_NO_SCALEX -DNO_NTSC -DNST_NO_XBR
        -D__PNES__ -D__PNES_VERSION_MAJOR__=${VERSION_MAJOR} -D__PNES_VERSION_MINOR__=${VERSION_MINOR}
        -Wno-unused-definition -Wno-unused-local-typedefs
        -Wno-sign-compare -Wno-unused-variable
        -Wno-delete-non-virtual-dtor -Wno-reorder
        -Wno-parentheses -Wno-unknown-pragmas
        -Wno-format-truncation -Wno-unused-but-set-variable
        -Wno-format-zero-length
        )

##############
# pnes
##############
set(PNES_INC sources)
file(GLOB PNES_SRC sources/*.c*)

##############
# deps
##############
include(FindPkgConfig)
pkg_search_module(LIBARCHIVE REQUIRED libarchive)
list(APPEND NESTOPIA_LDFLAGS ${LIBARCHIVE_LIBRARIES})

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # PS VITA PLATFORM
    #####################
    set(TITLE_ID "PNES00001" CACHE STRING "" FORCE)
    list(APPEND PNES_INC sources/vita)
    list(APPEND NESTOPIA_SRC sources/vita/port.c)
    list(APPEND NESTOPIA_CFLAGS -Dstrcasecmp=strcmp -DNST_DATADIR="app0:")
    list(APPEND NESTOPIA_LDFLAGS ScePower_stub)
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pNES - Portable Nestopia")
    set(PS4_PKG_TITLE_ID "PNES00001")
    set(PS4_PKG_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
    list(APPEND NESTOPIA_CFLAGS -DNST_DATADIR="/app0")
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATFORM
    #####################
    list(APPEND NESTOPIA_SRC sources/switch/port.c)
    list(APPEND NESTOPIA_CFLAGS -DNST_DATADIR="romfs:")
elseif (PLATFORM_3DS)
    #####################
    # 3DS PLATFORM
    #####################
    list(APPEND PNES_INC sources/3ds)
    list(APPEND NESTOPIA_CFLAGS -Dstrcasecmp=strcmp -DNST_DATADIR="romfs:")
    list(APPEND NESTOPIA_SRC sources/3ds/port.c)
elseif (PLATFORM_LINUX)
    #####################
    # LINUX
    #####################
    list(APPEND NESTOPIA_CFLAGS -DNST_DATADIR="/tmp/c2d_romfs")
elseif (PLATFORM_WINDOWS)
    #####################
    # WINDOWS
    list(APPEND NESTOPIA_CFLAGS -DNST_DATADIR="data_romfs")
    list(APPEND PNES_INC sources/windows)
endif (PLATFORM_VITA)

#####################
# pnes executable
#####################
# add main executable (elf)
add_executable(${PROJECT_NAME} ${PNES_SRC} ${NESTOPIA_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PNES_INC} ${NESTOPIA_INC})
target_compile_options(${PROJECT_NAME} PRIVATE ${NESTOPIA_CFLAGS})
target_link_libraries(${PROJECT_NAME} cross2dui ${NESTOPIA_LDFLAGS})

# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)

# add common skin data
add_custom_target(${PROJECT_NAME}.data.skin
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/common/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/${TARGET_PLATFORM}/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/common/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_directory_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_CURRENT_SOURCE_DIR}/../data/${TARGET_PLATFORM}/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_CURRENT_LIST_DIR}/../libcross2d/cmake/copy_directory_custom.cmake
        )
add_dependencies(${PROJECT_NAME}.data.skin ${PROJECT_NAME}.data)
if (PLATFORM_LINUX)
    add_dependencies(${PROJECT_NAME}-romfs ${PROJECT_NAME}.data.skin)
else ()
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.skin)
endif ()

# add extra data to libcross2d default targets data (nestopia database and zipped skin)
add_custom_target(${PROJECT_NAME}.data.extra
        DEPENDS ${PROJECT_NAME}.data
        # copy nst database from nestopia
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR}/data_romfs/
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra)

