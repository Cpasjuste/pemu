cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE ON)

project(pfba)

# handle m68k
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/deps)
ADD_CUSTOM_COMMAND(OUTPUT
        ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.h ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.c
        COMMAND gcc ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/m68kmake.c -o ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kmake
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kmake ${CMAKE_CURRENT_BINARY_DIR}/deps/ ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/m68k_in.c)

##############
# burn
##############
set(BURN_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/devices
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/snd
        )
file(GLOB SRC_BURN
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/devices/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/snd/*.c*
        )
##############
# burner
##############
set(BURNER_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner
        )
file(GLOB SRC_BURNER
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/*.c*
        )
list(REMOVE_ITEM SRC_BURNER
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/conc.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/cong.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/dat.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/gamc.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/gami.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/image.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/misc.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/sshot.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/state.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/unzip.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/zipfn.cpp
        )
##############
# drivers
##############
set(DRV_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/atari
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/capcom
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/cave
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/coleco
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/cps3
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/dataeast
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/galaxian
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/irem
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/konami
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/megadrive
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/midway
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/msx
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/neogeo
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/pce
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/pgm
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/pre90s
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/psikyo
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/pst90s
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/sega
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/sg1000
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/sms
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/snes
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/spectrum
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/taito
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/toaplan
        )
file(GLOB_RECURSE SRC_DRV
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/*.cpp
        )
list(REMOVE_ITEM SRC_DRV
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/atari/WHATSNEW.txt.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/capcom/ctv_make.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/pgm/pgm_sprite_create.cpp
        )

##############
# interfaces
##############
set(INTF_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/input
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/cd
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/audio
        )
file(GLOB SRC_INTF
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/input/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/cd/*.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/audio/*.cpp
        )
list(REMOVE_ITEM SRC_INTF
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/input/inp_interface.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/audio/aud_dsp.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/intf/audio/aud_interface.cpp
        )

##############
# cpu
##############
set(CPU_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu
        #${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/a68k
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/adsp2100
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tms34010
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/arm
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/arm7
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/h6280
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/hd6309
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8039
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8x41
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8051
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/konami
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6800
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6805
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6809
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/nec
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/pic16c5x
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/s2650
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tlcs90
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/sh2
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tms32010
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/upd7725
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/upd7810
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z80
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180
        )
file(GLOB SRC_CPU
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/*.c*
        #${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/a68k/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/adsp2100/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tms34010/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/arm/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/arm7/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/h6280/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/hd6309/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8039/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8x41/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8051/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/konami/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6800/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6805/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6809/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/nec/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/pic16c5x/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/s2650/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tlcs90/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/sh2/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tms32010/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/upd7725/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/upd7810/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z80/*.c*
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/*.c*
        # PFBA
        #pfba/cpu/c68000_intf.cpp
        )
list(REMOVE_ITEM SRC_CPU
        #${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68000_intf.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/adsp2100/2100ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tms34010/tms34010_dasm.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/tms34010/tms34010_newdasm.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/m68kmake.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/m68k_in.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/m68kfpu.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m68k/m68kdasm.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/arm7/arm7core.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/arm7/arm7exec.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/hd6309/6309tbl.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/h6280/tblh6280.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/hd6309/6309ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/konami/konamops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502/t6502.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502/t65c02.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502/t65sc02.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502/tdeco16.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6502/tn2a03.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/konami/konamtbl.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6800/6800ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6800/6800tbl.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6805/6805ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6809/6809ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/m6809/6809tbl.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3/mips3_dasm.cpp
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/nec/necinstr.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/nec/v25sfr.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/nec/v25instr.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8051/mcs51ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/i8x41/8x41dasm.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/upd7810/7810ops.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/upd7810/7810tbl.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op12.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/am.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/am1.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/am2.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op7a.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/am3.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op2.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op4.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op6.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op3.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/op5.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/optable.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/v60mem.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/v60/v60d.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/z180cb.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/z180dd.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/z180ed.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/z180fd.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/z180op.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/z180/z180xy.c
        )

##############
# 7z
##############
set(7Z_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z
        )
file(GLOB SRC_7Z
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/7zDec.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/7zArcIn.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/7zStream.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/LzmaDec.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/Lzma2Dec.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/Bra.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/Bcj2.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/7zBuf.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/Delta.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/Bra86.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/BraIA64.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/7zCrc.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/7zCrcOpt.c
        ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/libs/lib7z/CpuArch.c
        )

##############
# pfba
##############
set(PFBA_DIRS sources sources/fba)
file(GLOB SRC_PFBA sources/*.c* sources/fba/*.c*)

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
        -DUSE_SPEEDHACKS -DBUILD_SDL -DBUILD_M68K
        -Dstricmp=strcmp
        -Wno-write-strings -Wno-narrowing -fforce-addr -finline-limit=1200 -Wno-sign-compare
        -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sequence-point
        )

if (NOT BUILD_DEBUG)
    list(APPEND FLAGS -O3)
endif ()

if (OPTION_BUILTIN_MINIZIP)
    list(REMOVE_ITEM SRC_BURNER ${CMAKE_SOURCE_DIR}/cores/fba/src/burner/ioapi.c)
else ()
    set(LDFLAGS minizip)
endif ()

#################
# PSP2 (ps vita)
#################
if (PLATFORM_VITA)
    set(TITLE_ID "PFBA00001" CACHE STRING "" FORCE)
    #list(APPEND INC pfba/cpu/cyclone)
    #list(APPEND SRC_PFBA pfba/cpu/cyclone/Cyclone.s)
    list(APPEND FLAGS -DNO_KEYBOARD -DBUILD_C68K -DUSE_FILE32API -DLSB_FIRST)
    #-D__PFBA_ARM__
    if (BUILD_DEBUG)
        list(APPEND FLAGS
                -D__PSP2_DEBUG__ -g # TODO: cyclone arm asm core not working without O2/O3
                )
    else ()
        list(APPEND FLAGS
                -fno-asynchronous-unwind-tables -funroll-loops
                -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
                -mvectorize-with-neon-quad -funsafe-math-optimizations
                -mlittle-endian -munaligned-access
                -fsingle-precision-constant
                )
    endif (BUILD_DEBUG)
    list(APPEND LDFLAGS ScePower_stub)
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
            -DNO_KEYBOARD -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
elseif (PLATFORM_LINUX)
    ##############
    # SDL2
    ##############
    list(APPEND FLAGS -DLSB_FIRST)
    if (PLATFORM_WINDOWS)
        list(APPEND LDFLAGS z)
    else ()
        list(APPEND FLAGS -D__fastcall=)
    endif ()
    if (PLATFORM_LDK)
        list(REMOVE_ITEM DRV_DIRS ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/midway)
        list(REMOVE_ITEM SRC_DRV
                ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/midway/d_kinst.cpp
                ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/midway/d_wunit.cpp
                ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/midway/midwunit.cpp
                )
        list(REMOVE_ITEM SRC_CPU
                ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3/mips3.cpp
                ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3/cop0.cpp
                ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3/cop1.cpp
                ${CMAKE_SOURCE_DIR}/cores/fba/src/cpu/mips3_intf.cpp
                )
    endif ()
endif (PLATFORM_VITA)

# source files
set(SRC ${SRC_PFBA} ${SRC_CPU} ${SRC_DRV} ${SRC_BURN} ${SRC_BURNER} ${SRC_INTF} ${SRC_7Z})

#####################
# pfba executable
#####################
add_executable(${PROJECT_NAME} ${SRC} ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.c)
target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++11 -fpermissive>)
target_include_directories(${PROJECT_NAME} PRIVATE ${INC})
target_link_libraries(${PROJECT_NAME} cross2dui ${LDFLAGS})
# include libcross2d targets
include(${CMAKE_SOURCE_DIR}/libcross2d/cmake/targets.cmake)

# fba deps
find_package(Perl REQUIRED)
add_custom_target(${PROJECT_NAME}.deps
        #COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/deps
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/deps
        # ctv.h
        COMMAND g++ ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/capcom/ctv_make.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/deps/ctv_make
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/deps/ctv_make > ${CMAKE_CURRENT_BINARY_DIR}/deps/ctv.h
        # pgm sprite
        COMMAND g++ ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/pgm/pgm_sprite_create.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/deps/pgm_sprite_create
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/deps/pgm_sprite_create > ${CMAKE_CURRENT_BINARY_DIR}/deps/pgm_sprite.h
        # func headers
        COMMAND perl ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/toa_gp9001_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/toa_gp9001_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/neo_sprite_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/neo_sprite_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/cave_tile_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/cave_tile_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/cave_sprite_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/cave_sprite_func.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/psikyo_tile_func.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/psikyo_tile_func.h
        )
message("-- C2D: Be sure to build \"${PROJECT_NAME}.deps\" target before \"${PROJECT_NAME}\" target")
# driverlist.h
# TODO: crap, mingw64 perl can't process this on windows... ( Argument list too long)
if (PLATFORM_WINDOWS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/pfba/data/windows)
else ()
    add_custom_target(${PROJECT_NAME}.deps.gamelist
            COMMAND ${PERL_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/gamelist.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/driverlist.h -l ${CMAKE_CURRENT_BINARY_DIR}/deps/gamelist.txt ${SRC_DRV}
            )
    add_dependencies(${PROJECT_NAME}.deps ${PROJECT_NAME}.deps.gamelist)
endif ()

# add extra data to libcross2d default targets data (nestopia database and zipped skin)
add_custom_target(${PROJECT_NAME}.data.extra
        DEPENDS ${PROJECT_NAME}.data
        # make dummy directory for user files
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/roms
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/titles
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/previews
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/icons
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/hiscores
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/hiscores
        # copy hiscore database from pfba
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/pfba/data/hiscore.dat ${CMAKE_CURRENT_BINARY_DIR}/hiscores/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/pfba/data/hiscore.dat ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/hiscores/
        # compress skin
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default.zip .
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        # remove non-zipped skin from data directory (used for program execution from cmake build) before zipping it for release
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default

        # compress default_320x240 skin
        COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240 && ${ZIP} -ru ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240.zip .
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240.zip ${CMAKE_CURRENT_BINARY_DIR}/skins/
        # remove non-zipped skin from data directory (used for program execution from cmake build) before zipping it for release
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/skins/default_320x240
        )
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra)
add_dependencies(${PROJECT_NAME}_${TARGET_PLATFORM}_release ${PROJECT_NAME}.data.extra)

if (PLATFORM_WINDOWS)
    # TODO: fix curl/nghttp2 static linking...
    add_custom_target(${PROJECT_NAME}.data.extra.win64
            DEPENDS ${PROJECT_NAME}.data.extra
            COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/*.dll
            COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/*.dll
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libwinpthread-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libnghttp2-14.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libunistring-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libstdc++-6.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libiconv-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libgcc_s_seh-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libass-9.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avcodec-58.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avfilter-7.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avformat-58.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avutil-56.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/swresample-3.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/swscale-5.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/SDL2.dll" ${CMAKE_CURRENT_BINARY_DIR}/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libwinpthread-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libnghttp2-14.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libunistring-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libstdc++-6.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libiconv-2.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libgcc_s_seh-1.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/libass-9.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avcodec-58.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avfilter-7.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avformat-58.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/avutil-56.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/swresample-3.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/swscale-5.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            COMMAND ${CMAKE_COMMAND} -E copy "/mingw64/bin/SDL2.dll" ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/
            )
    add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}.data.extra.win64)
endif ()

#############
# install
#############
install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        PERMISSIONS OWNER_EXECUTE OWNER_READ
        DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data_romfs/ DESTINATION share/pfba)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data_datadir/ DESTINATION share/pfba)
