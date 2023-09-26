cmake_minimum_required(VERSION 3.5)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pgba)

##################
# mGBA
##################
set(LIBMGBA_ONLY ON CACHE BOOL "")
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cores/mgba build)
set_property(TARGET mgba APPEND PROPERTY COMPILE_DEFINITIONS "_GNU_SOURCE;IOAPI_NO_64;COLOR_16_BIT;COLOR_5_6_5;DISABLE_THREADING")

##############
# pgba
##############
set(PGBA_INC sources)
file(GLOB PGBA_SRC sources/*.c*)
list(APPEND PGBA_SRC
        ../main.cpp
        ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/util/vfs/vfs-fd.c
        ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/util/vfs/vfs-dirent.c
)
set(GBA_CFLAGS
        -D__PGBA__ -D__PEMU_VERSION_MAJOR__=${VERSION_MAJOR} -D__PEMU_VERSION_MINOR__=${VERSION_MINOR}
)

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
    set_property(TARGET mgba PROPERTY POSITION_INDEPENDENT_CODE ON)
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
    if (ANDROID_ABI STREQUAL armeabi-v7a)
        list(APPEND PGEN_CFLAGS -marm -mcpu=cortex-a7 -mfpu=neon
                -ffunction-sections -fomit-frame-pointer) # fix crash with release optimizations
    endif ()
    list(APPEND GENESIS_CFLAGS -DHAVE_SYS_PARAM_H)
endif (PLATFORM_VITA)

#####################
# pgba executable
#####################
# add main executable (elf)
add_executable(${PROJECT_NAME} ${PGBA_SRC} ${GBA_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${PGBA_INC} ${GBA_INC})
target_compile_options(${PROJECT_NAME} PRIVATE ${GBA_CFLAGS} ${PGBA_CFLAGS})
target_link_libraries(${PROJECT_NAME} cross2dui ${GBA_LDFLAGS} ${PGBA_LDFLAGS} mgba)

# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/external/libcross2d/cmake/targets.cmake)

# add common skin data
add_custom_target(${PROJECT_NAME}.data.skin
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/data/common/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_SOURCE_DIR}/external/libcross2d/cmake/copy_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/data/${TARGET_PLATFORM}/datadir -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_datadir -P ${CMAKE_SOURCE_DIR}/external/libcross2d/cmake/copy_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/data/common/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_SOURCE_DIR}/external/libcross2d/cmake/copy_custom.cmake
        COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_SOURCE_DIR}/data/${TARGET_PLATFORM}/romfs -D DST=${CMAKE_CURRENT_BINARY_DIR}/data_romfs -P ${CMAKE_SOURCE_DIR}/external/libcross2d/cmake/copy_custom.cmake
)
add_dependencies(${PROJECT_NAME}.data.skin ${PROJECT_NAME}.data)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.skin)
