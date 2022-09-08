cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pgba)

#set(OPTION_CHD ON CACHE STRING "build with CHD support")

##################
# mGBA
##################
set(GBA_INC
        ${CMAKE_SOURCE_DIR}/cores/mgba/include
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/arm
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/core
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/gb
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/gba
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/gba/cheats
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/sm83
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/util
        )

file(GLOB GBA_SRC
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/arm/*.c*
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/core/*.c*
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/gb/*.c*
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/gba/*.c*
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/gba/cheats/*.c*
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/sm83/*.c*
        ${CMAKE_SOURCE_DIR}/cores/mgba/src/util/*.c*
        )

set(GBA_CFLAGS
        -DHAVE_LOCALE
        -D__PGBA__ -D__PGBA_VERSION_MAJOR__=${VERSION_MAJOR} -D__PGBA_VERSION_MINOR__=${VERSION_MINOR}
        )

##############
# pgba
##############
set(PGBA_INC sources)
file(GLOB PGBA_SRC sources/*.c*)

#####################
# PLATORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # PS VITA PLATORM
    #####################
    set(TITLE_ID "PGBA00001" CACHE STRING "" FORCE)
    list(APPEND PGBA_INC)
    list(APPEND PGBA_SRC)
    list(APPEND PGBA_CFLAGS)
    list(APPEND PGBA_LDFLAGS)
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pGBA - Portable GameBoy Advance")
    set(PS4_PKG_TITLE_ID "PGBA00001")
    set(PS4_PKG_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
    list(APPEND GBA_CFLAGS -DHAVE_SYS_PARAM_H)
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATORM
    #####################
    list(APPEND PGBA_SRC)
    list(APPEND PGBA_CFLAGS)
elseif (PLATFORM_3DS)
elseif (PLATFORM_LINUX OR PLATFORM_WINDOWS)
    #####################
    # SDL2 PLATORM
    #####################
    list(APPEND PGBA_CFLAGS)
elseif (PLATFORM_ANDROID)
    #####################
    # ANDROID PLATORM
    #####################
    list(APPEND GBA_CFLAGS -DHAVE_SYS_PARAM_H)
endif (PLATFORM_VITA)

#####################
# pgen executable
#####################
# add main executable (elf)
add_executable(${PROJECT_NAME} ${PGBA_SRC} ${GBA_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PGBA_INC} ${GBA_INC})
target_compile_options(${PROJECT_NAME} PRIVATE ${GBA_CFLAGS} ${PGBA_CFLAGS})
target_link_libraries(${PROJECT_NAME} cross2dui ${GBA_LDFLAGS} ${PGBA_LDFLAGS})

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
