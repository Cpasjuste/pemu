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
endif (BUILD_DEBUG)

##############
# pnes
##############
set(PNES_INC sources)
file(GLOB PNES_SRC sources/*.c*)

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
    list(APPEND NESTOPIA_SRC sources/switch/port.c)
elseif (BUILD_LINUX)
    #####################
    # SDL2 PLATORM
    #####################
    if (BUILD_WINDOWS)
        #list(APPEND NESTOPIA_CFLAGS -D_MINGW)
    endif (BUILD_WINDOWS)
endif (BUILD_VITA)

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
include(${CMAKE_SOURCE_DIR}/libcross2dui/libcross2d/Targets.cmake)

# add extra data to libcross2d default targets data (nestopia database and zipped skin)
add_custom_target(${PROJECT_NAME}.data.extra
        DEPENDS ${PROJECT_NAME}.data
        # make dummy directory for user files
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/roms
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/titles
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/previews
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/icons
        # copy nst database from nestopia
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cores/nestopia/NstDatabase.xml ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/
        # compress skin
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/skins/default && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/skins/default.zip .
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/skins/default.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        # remove non-zipped skin from data directory (used for program execution from cmake build) before zipping it for release
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_read_write/skins/default
        )
add_dependencies(${PROJECT_NAME}_${TARGET_PLATFORM}_release ${PROJECT_NAME}.data.extra)
