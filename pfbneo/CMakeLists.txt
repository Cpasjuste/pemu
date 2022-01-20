cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pfbneo)

# handle m68k
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/deps)
ADD_CUSTOM_COMMAND(OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.h ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.c
        COMMAND gcc ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m68k/m68kmake.c -o ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kmake
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kmake ${CMAKE_CURRENT_BINARY_DIR}/deps/ ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m68k/m68k_in.c)

##############
# burn
##############
set(BURN_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/devices
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/snd
        )
file(GLOB SRC_BURN
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/devices/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/snd/*.c*
        )
##############
# burner
##############
set(BURNER_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/sdl
        )
file(GLOB SRC_BURNER
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/sdl/inpdipsw.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/sdl/input_sdl2.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/sdl/neocdlist.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/sdl/stringset.cpp
        )
list(REMOVE_ITEM SRC_BURNER
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/conc.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/cong.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/dat.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/image.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/sshot.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/state.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/unzip.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/zipfn.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/luaengine.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/luasav.cpp
        )

##############
# drivers
##############
set(DRV_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/atari
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/capcom
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/cave
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/channelf
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/coleco
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/cps3
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/dataeast
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/galaxian
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/irem
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/konami
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/megadrive
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/midway
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/pce
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/pst90s
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/pre90s
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/neogeo
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/nes
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/pgm
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/psikyo
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/sega
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/sg1000
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/sms
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/msx
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/spectrum
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/taito
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/toaplan
        )
file(GLOB_RECURSE SRC_DRV
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/*.cpp
        )
list(REMOVE_ITEM SRC_DRV
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/capcom/ctv_make.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/pgm/pgm_sprite_create.cpp
        )

##############
# interfaces
##############
set(INTF_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/input
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/cd
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/cd/sdl
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/audio
        )
file(GLOB SRC_INTF
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/input/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/cd/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/cd/sdl/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/audio/*.cpp
        )
list(REMOVE_ITEM SRC_INTF
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/audio/aud_dsp.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/intf/audio/aud_interface.cpp
        )

##############
# cpu
##############
set(CPU_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu
        #${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/a68k
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/arm
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/arm7
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/e132xs
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/f8
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/h6280
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/hd6309
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/i386
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/i8039
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/i8x41
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/i8051
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/adsp2100
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/konami
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m377
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/mips3
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m68k
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m6502
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m6800
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m6805
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m6809
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/nec
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/pic16c5x
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/s2650
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tlcs90
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tlcs900
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/sh2
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tms32010
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tms34
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/upd7725
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/upd7810
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/v60
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/z80
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/z180
        )
file(GLOB_RECURSE SRC_CPU
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m68k/m68kcpu.c
        )
list(REMOVE_ITEM SRC_CPU
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tms34010_intf.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tms34010/tms34010_newdasm.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/tms34010/tms34010_dasm.cpp
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/mips3/x64/mips3_x64.cpp
        )

##############
# 7z
##############
set(7Z_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z
        )
file(GLOB SRC_7Z
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/7zDec.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/7zArcIn.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/7zStream.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/LzmaDec.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/Lzma2Dec.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/Bra.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/Bcj2.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/7zBuf.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/Delta.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/Bra86.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/BraIA64.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/7zCrc.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/7zCrcOpt.c
        ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/libs/lib7z/CpuArch.c
        )

##############
# pfbneo
##############
set(PFBA_DIRS sources sources/fbneo)
file(GLOB SRC_PFBA sources/*.c* sources/fbneo/*.c*)

# includes
set(INC ${PFBA_DIRS}
        ${7Z_DIRS}
        ${BURN_DIRS}
        ${BURNER_DIRS}
        ${DRV_DIRS}
        ${CPU_DIRS}
        ${INTF_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}/deps
        )

# flags
set(FLAGS
        -D__PFBA__
        -D__PFBA_VERSION_MAJOR__=${VERSION_MAJOR}
        -D__PFBA_VERSION_MINOR__=${VERSION_MINOR}
        -DINCLUDE_7Z_SUPPORT -DMINIZIP_LIB
        -DUSE_SPEEDHACKS -DBUILD_SDL2 -DBUILD_M68K
        -Wno-write-strings -Wno-narrowing -fforce-addr -finline-limit=1200 -Wno-sign-compare
        -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sequence-point
        )

set(CMAKE_CXX_STANDARD 17)

if (OPTION_BUILTIN_MINIZIP)
    list(REMOVE_ITEM SRC_BURNER ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burner/ioapi.c)
else ()
    set(LDFLAGS minizip)
endif ()

#################
# PSP2 (ps vita)
#################
if (PLATFORM_VITA)
    enable_language(ASM)
    set(TITLE_ID "PFBA00001" CACHE STRING "" FORCE)
    list(REMOVE_ITEM SRC_CPU ${CMAKE_SOURCE_DIR}/cores/fbneo/src/cpu/m68000_intf.cpp)
    list(APPEND INC
            sources/cpu/cyclone
            ${VITASDK}/arm-vita-eabi/include/SDL2/
            )
    list(APPEND SRC_PFBA sources/cpu/c68000_intf.cpp sources/cpu/cyclone/Cyclone.s)
    list(APPEND FLAGS -DBUILD_C68K -DUSE_FILE32API -DLSB_FIRST -D__PFBA_ARM__)
    list(APPEND LDFLAGS ScePower_stub pthread)
elseif (PLATFORM_PS4)
    set(PS4_PKG_TITLE "pFBA - Portable Final Burn Neo")
    set(PS4_PKG_TITLE_ID "PFBN00001")
    set(PS4_PKG_VERSION "05.00")
    list(APPEND FLAGS -D__fastcall= -DLSB_FIRST)
    list(APPEND LDFLAGS)
elseif (PLATFORM_PS3)
    ##############
    # PS3
    ##############
    list(APPEND FLAGS
            -D__PS3__
            -DNO_KEYBOARD -D__CELLOS_LV2__ -DUSE_FILE32API
            -mhard-float -ffunction-sections -fdata-sections)
    list(APPEND LDFLAGS
            ${PSL1GHT}/ppu/lib/libpngdec.a
            ${PSL1GHT}/portlibs/ppu/lib/libfreetype.a
            ${PSL1GHT}/portlibs/ppu/lib/libpng.a
            ${PSL1GHT}/portlibs/ppu/lib/libtiff.a
            ${PSL1GHT}/portlibs/ppu/lib/libjpeg.a
            ${PSL1GHT}/ppu/lib/libgcm_sys.a
            ${PSL1GHT}/ppu/lib/librsx.a
            ${PSL1GHT}/ppu/lib/libsysutil.a
            ${PSL1GHT}/ppu/lib/libsysmodule.a
            ${PSL1GHT}/ppu/lib/libio.a
            ${PSL1GHT}/ppu/lib/libaudio.a
            ${PSL1GHT}/portlibs/ppu/lib/libz.a)
elseif (PLATFORM_3DS)
    ##############
    # 3DS
    ##############
    list(APPEND INC
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/armv6k/include)
    list(APPEND FLAGS -DARM11 -D_3DS -D__3DS__
            -DNO_KEYBOARD -D__PFBA_ARM__ -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
    list(APPEND LDFLAGS
            ${DEVKITPRO}/portlibs/armv6k/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libz.a
            ${DEVKITPRO}/libctru/lib/libcitro3d.a
            ${DEVKITPRO}/libctru/lib/libctru.a
            m)
elseif (PLATFORM_SWITCH)
    ##############
    # NX (SWITCH)
    ##############
    list(APPEND FLAGS
            -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
elseif (PLATFORM_LINUX)
    ##############
    # SDL2
    ##############
    list(APPEND FLAGS -DLSB_FIRST)
    if (PLATFORM_WINDOWS)
        list(APPEND LDFLAGS z -static)
    else ()
        list(APPEND FLAGS -D__fastcall=)
    endif ()
endif (PLATFORM_VITA)

# source files
set(SRC ${SRC_PFBA} ${SRC_CPU} ${SRC_DRV} ${SRC_BURN} ${SRC_BURNER} ${SRC_INTF} ${SRC_7Z})

#####################
# pfbneo executable
#####################
add_executable(${PROJECT_NAME} ${SRC} ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.c)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC})
target_link_libraries(${PROJECT_NAME} cross2dui cross2d ${LDFLAGS})
# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)

# fbneo deps
find_package(Perl REQUIRED)
add_custom_target(${PROJECT_NAME}.deps
        #COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/deps
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/deps
        # ctv.h
        COMMAND g++ ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/capcom/ctv_make.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/deps/ctv_make
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/deps/ctv_make > ${CMAKE_CURRENT_BINARY_DIR}/deps/ctv.h
        # pgm sprite
        COMMAND g++ ${CMAKE_SOURCE_DIR}/cores/fbneo/src/burn/drv/pgm/pgm_sprite_create.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/deps/pgm_sprite_create
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/deps/pgm_sprite_create > ${CMAKE_CURRENT_BINARY_DIR}/deps/pgm_sprite.h
        # func headers
        COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/scripts/toa_gp9001_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/toa_gp9001_func.h
        COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/scripts/neo_sprite_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/neo_sprite_func.h
        COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/scripts/cave_tile_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/cave_tile_func.h
        COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/scripts/cave_sprite_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/cave_sprite_func.h
        COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/scripts/psikyo_tile_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/psikyo_tile_func.h
        )
message("-- C2D: Be sure to build \"${PROJECT_NAME}.deps\" target before \"${PROJECT_NAME}\" target")
# driverlist.h
# TODO: crap, mingw64 perl can't process this on windows... ( Argument list too long)
if (PLATFORM_WINDOWS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/pfbneo/data/windows)
else ()
    add_custom_target(${PROJECT_NAME}.deps.gamelist
            COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fbneo/src/dep/scripts/gamelist.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/driverlist.h -l ${CMAKE_CURRENT_BINARY_DIR}/deps/gamelist.txt ${SRC_DRV}
            )
    add_dependencies(${PROJECT_NAME}.deps ${PROJECT_NAME}.deps.gamelist)
endif ()

if (PLATFORM_WINDOWS)
    add_custom_command(TARGET ${PROJECT_NAME}.data POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/pfbneo/data/windows/copy_deps.sh ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMENT "Copying required mingw dependencies..."
            )
endif ()

