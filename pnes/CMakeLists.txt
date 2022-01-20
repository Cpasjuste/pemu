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
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/*.c*
        )

list(REMOVE_ITEM NESTOPIA_SRC
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/audio.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/video.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/config.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/common/sdlinput.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilter2xSaI.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterHqX.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterScaleX.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterxBR.cpp
        ${CMAKE_SOURCE_DIR}/cores/nestopia/source/core/NstVideoFilterNtsc.cpp
        )

set(NESTOPIA_CFLAGS
        -DVERSION="1.0"
        -D__LIBRETRO__ -D_MINGW -D__NST_UNZIP__
        -DNST_NO_2XSAI -DNST_NO_HQ2X -DNST_NO_SCALEX -DNO_NTSC -DNST_NO_XBR
        -D__PNES__ -D__PNES_VERSION_MAJOR__=${VERSION_MAJOR} -D__PNES_VERSION_MINOR__=${VERSION_MINOR}
        -Wno-unused-definition -Wno-unused-local-typedefs
        -Wno-sign-compare -Wno-unused-variable
        -Wno-delete-non-virtual-dtor -Wno-reorder
        -Wno-parentheses -Wno-unknown-pragmas
        -Wno-format-truncation -Wno-unused-but-set-variable
        -Wno-format-zero-length
        )

if (NOT OPTION_BUILTIN_MINIZIP)
    set(NESTOPIA_LDFLAGS minizip)
endif ()

##############
# pnes
##############
set(PNES_INC sources)
file(GLOB PNES_SRC sources/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # PS VITA PLATORM
    #####################
    set(TITLE_ID "PNES00001" CACHE STRING "" FORCE)
    list(APPEND PNES_INC sources/vita)
    list(APPEND NESTOPIA_SRC sources/vita/port.c)
    list(APPEND NESTOPIA_CFLAGS -Dstrcasecmp=strcmp)
    list(APPEND NESTOPIA_LDFLAGS ScePower_stub)
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pNES - Portable Nestopia")
    set(PS4_PKG_TITLE_ID "PNES00001")
    set(PS4_PKG_VERSION "05.00")
    list(APPEND NESTOPIA_CFLAGS -DDATADIR="/app0/")
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND NESTOPIA_SRC sources/switch/port.c)
    list(APPEND NESTOPIA_CFLAGS -DDATADIR="romfs:/")
elseif (PLATFORM_3DS)
    #####################
    # 3DS PLATORM
    #####################
    list(APPEND NESTOPIA_CFLAGS -Dstrcasecmp=strcmp)
    list(APPEND NESTOPIA_SRC sources/3ds/port.c)
elseif (PLATFORM_LINUX)
    #####################
    # SDL2 PLATORM
    #####################
    list(APPEND NESTOPIA_CFLAGS -DDATADIR="romfs:/")
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

# add extra data to libcross2d default targets data (nestopia database and zipped skin)
add_custom_target(${PROJECT_NAME}.data.extra
        DEPENDS ${PROJECT_NAME}.data
        # copy nst database from nestopia
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR}/data_romfs/
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra)

#[[
if (PLATFORM_WINDOWS)
    # TODO: fix curl/nghttp2 static linking...
    add_custom_target(${PROJECT_NAME}.data.extra.win64
            DEPENDS ${PROJECT_NAME}.data.extra
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libwinpthread-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libnghttp2-14.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libunistring-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libstdc++-6.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libiconv-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libgcc_s_seh-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libwinpthread-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libnghttp2-14.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libunistring-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libstdc++-6.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libiconv-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
            COMMAND ${CMAKE_COMMAND} -E copy "/msys64/mingw64/bin/libgcc_s_seh-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
            )
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra.win64)
endif ()
]]
