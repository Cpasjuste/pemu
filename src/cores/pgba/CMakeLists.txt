cmake_minimum_required(VERSION 3.10)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pgba)

##################
# mGBA
##################
set(LIBMGBA_ONLY ON CACHE BOOL "")
set(M_CORE_GB OFF CACHE BOOL "") # TODO
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cores/mgba build)
set_property(TARGET mgba APPEND PROPERTY COMPILE_DEFINITIONS
        "_GNU_SOURCE;IOAPI_NO_64;USE_LIBZIP;HAVE_CRC32;COLOR_16_BIT;COLOR_5_6_5;DISABLE_THREADING"
)
if (PLATFORM_VITA)
    set_property(TARGET mgba APPEND PROPERTY COMPILE_DEFINITIONS "PSP2")
endif ()

##############
# pgba
##############
set(PGBA_INC sources)
file(GLOB PGBA_SRC sources/*.c*)
list(APPEND PGBA_SRC
        ../main.cpp
        ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/util/vfs/vfs-fd.c
        ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/util/vfs/vfs-dirent.c
        ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/util/vfs/vfs-zip.c
)
set(GBA_CFLAGS
        -D__PGBA__ -D__PEMU_VERSION_MAJOR__=${VERSION_MAJOR} -D__PEMU_VERSION_MINOR__=${VERSION_MINOR}
)

#####################
# PLATFORM SPECIFIC
#####################
if (PLATFORM_VITA)
    #####################
    # PS VITA PLATFORM
    #####################
    set(TITLE_ID "PGBA00001" CACHE STRING "" FORCE)
    list(APPEND PGBA_INC)
    list(REMOVE_ITEM PGBA_SRC ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/util/vfs/vfs-dirent.c)
    list(APPEND PGBA_SRC ${CMAKE_SOURCE_DIR}/external/cores/mgba/src/platform/psp2/sce-vfs.c)
    list(APPEND PGBA_CFLAGS -DPSP2 -D__HAIKU__)
    list(APPEND PGBA_LDFLAGS)
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pGBA - Portable GameBoy Advance")
    set(PS4_PKG_TITLE_ID "PGBA00001")
    set(PS4_PKG_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
    list(APPEND GBA_CFLAGS -DHAVE_SYS_PARAM_H)
elseif (PLATFORM_SWITCH)
    #####################
    # SWITCH PLATFORM
    #####################
    set_property(TARGET mgba PROPERTY POSITION_INDEPENDENT_CODE ON) # -fPIC
    set_property(TARGET mgba APPEND PROPERTY INCLUDE_DIRECTORIES ${DEVKITPRO}/portlibs/switch/include) # for vfs-zip.c
    list(APPEND PGBA_CFLAGS)
elseif (PLATFORM_3DS)
elseif (PLATFORM_LINUX OR PLATFORM_WINDOWS)
    if (PLATFORM_WINDOWS)
        list(APPEND PGBA_INC ${CMAKE_CURRENT_SOURCE_DIR}/sources/windows) # for vfs-dirent.c (mkdir)
    endif ()
    #####################
    # SDL2 PLATFORM
    #####################
    list(APPEND PGBA_CFLAGS)
elseif (PLATFORM_ANDROID)
    #####################
    # ANDROID PLATFORM
    #####################
    if (ANDROID_ABI STREQUAL armeabi-v7a)
        list(APPEND PGBA_CFLAGS -marm -mcpu=cortex-a7 -mfpu=neon
                -ffunction-sections -fomit-frame-pointer) # fix crash with release optimizations
    endif ()
    list(APPEND PGBA_CFLAGS -DHAVE_SYS_PARAM_H)
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
