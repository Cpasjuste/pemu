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
        -D__PNES_VERSION_MAJOR__=${VERSION_MAJOR}
        -D__PNES_VERSION_MINOR__=${VERSION_MINOR}
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
else ()
    list(APPEND NESTOPIA_CFLAGS -g -O3)
endif()

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
    list(APPEND NESTOPIA_LDFLAGS ScePower_stub)
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND NESTOPIA_SRC sources/switch/port.c)
elseif (PLATFORM_3DS)
    #####################
    # 3DS PLATORM
    #####################
    list(APPEND NESTOPIA_SRC sources/3ds/port.c)
elseif (PLATFORM_LINUX)
    #####################
    # SDL2 PLATORM
    #####################
    if (PLATFORM_LDK)
        list(REMOVE_ITEM NESTOPIA_LDFLAGS minizip)
    endif ()
    if (PLATFORM_WINDOWS)
        #list(APPEND NESTOPIA_CFLAGS -D_MINGW)
    endif (PLATFORM_WINDOWS)
endif (PLATFORM_VITA)

#####################
# pnes executable
#####################
# add main executable (elf)
add_executable(${PROJECT_NAME} ${PNES_SRC} ${NESTOPIA_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PNES_INC} ${NESTOPIA_INC})
target_compile_options(${PROJECT_NAME} PRIVATE ${NESTOPIA_CFLAGS})
target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++14 -fexceptions>)
target_link_libraries(${PROJECT_NAME} cross2dui ${NESTOPIA_LDFLAGS})

# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)

# add extra data to libcross2d default targets data (nestopia database and zipped skin)
add_custom_target(${PROJECT_NAME}.data.extra
        DEPENDS ${PROJECT_NAME}.data
        # make dummy directory for user files
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/roms
        # copy nst database from nestopia
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
        # default skin
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default.zip .
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default
        # big_preview skin
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/big_preview && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/big_preview.zip .
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/big_preview.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/big_preview
        # compress default_320x240 skin
        #COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240 && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240.zip .
        #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        # remove non-zipped skin from data directory (used for program execution from cmake build) before zipping it for release
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra)

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
