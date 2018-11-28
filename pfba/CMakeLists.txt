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
        )
##############
# drivers
##############
set(DRV_DIRS
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv
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
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/taito
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/toaplan
        )
file(GLOB_RECURSE SRC_DRV
        ${CMAKE_SOURCE_DIR}/cores/fba/src/burn/drv/*.cpp
        )
list(REMOVE_ITEM SRC_DRV
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
set(PFBA_DIRS pfba pfba/fba)
file(GLOB SRC_PFBA pfba/*.c* pfba/fba/*.c*)

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
        -D__PFBA_VERSION_MAJOR__=${PFBA_VERSION_MAJOR}
        -D__PFBA_VERSION_MINOR__=${PFBA_VERSION_MINOR}
        -DINCLUDE_7Z_SUPPORT
        -DUSE_SPEEDHACKS -DBUILD_SDL -DBUILD_M68K
        -Wno-write-strings -Wno-narrowing -fforce-addr -finline-limit=1200 -Wno-sign-compare
        -DFASTCALL -U__fastcall -D_fastcall= -D__fastcall= -D__fastcall__=
        #"-DINLINE=static inline" "-DSH2_INLINE=static inline"
        -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sequence-point
        #-fcheck-new
        )

#################
# PSP2 (ps vita)
#################
if (BUILD_PSP2)
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
                -mfpu=neon -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -ffast-math
                -fno-asynchronous-unwind-tables -funroll-loops
                -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
                -mvectorize-with-neon-quad -funsafe-math-optimizations
                -mlittle-endian -munaligned-access
                -fsingle-precision-constant
                )
    endif (BUILD_DEBUG)
    set(LDFLAGS ScePower_stub)
elseif (BUILD_PS3)
    ##############
    # PS3
    ##############
    list(APPEND FLAGS
            -D__PS3__
            -DNO_KEYBOARD -D__CELLOS_LV2__ -DUSE_FILE32API
            -mhard-float -ffunction-sections -fdata-sections)
    set(LDFLAGS
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
elseif (BUILD_3DS)
    ##############
    # 3DS
    ##############
    list(APPEND INC
            ${DEVKITPRO}/libctru/include
            ${DEVKITPRO}/portlibs/armv6k/include)
    list(APPEND FLAGS -DARM11 -D_3DS -D__3DS__
            -DNO_KEYBOARD -D__PFBA_ARM__ -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
    set(LDFLAGS
            ${DEVKITPRO}/portlibs/armv6k/lib/libpng.a
            ${DEVKITPRO}/portlibs/armv6k/lib/libz.a
            ${DEVKITPRO}/libctru/lib/libcitro3d.a
            ${DEVKITPRO}/libctru/lib/libctru.a
            m)
elseif (BUILD_SWITCH)
    ##############
    # NX (SWITCH)
    ##############
    list(APPEND FLAGS
            -DNO_KEYBOARD -DUSE_FILE32API -DLSB_FIRST
            -ffunction-sections -fomit-frame-pointer)
elseif (BUILD_SDL2)
    ##############
    # SDL2
    ##############
    list(APPEND FLAGS -DLSB_FIRST)
elseif (BUILD_SDL1)
    ##############
    # SDL1
    ##############
    list(APPEND FLAGS -DLSB_FIRST)
elseif (BUILD_SFML)
    ##############
    # SFML
    ##############
    list(APPEND FLAGS -DLSB_FIRST)
endif (BUILD_PSP2)

# source files
set(SRC ${SRC_PFBA} ${SRC_CPU} ${SRC_DRV} ${SRC_BURN} ${SRC_BURNER} ${SRC_INTF} ${SRC_7Z})

############
# FBA deps
############
add_custom_target(${PROJECT_NAME}.deps
        COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/deps && mkdir ${CMAKE_CURRENT_BINARY_DIR}/deps
        # driverlist.h
        COMMAND perl ${CMAKE_SOURCE_DIR}/cores/fba/src/dep/scripts/gamelist.pl -o ${CMAKE_CURRENT_BINARY_DIR}/deps/driverlist.h -l ${CMAKE_CURRENT_BINARY_DIR}/deps/gamelist.txt ${SRC_DRV}
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

##########################
# main target
##########################
add_custom_target(${PROJECT_NAME}.data
        COMMAND cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/pfba/data/skin ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/hiscores
        COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/pfba/data/hiscore.dat ${CMAKE_CURRENT_BINARY_DIR}/hiscores
        )
add_executable(${PROJECT_NAME}.elf ${SRC} ${CMAKE_CURRENT_BINARY_DIR}/deps/m68kops.c)
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${FLAGS})
target_compile_options(${PROJECT_NAME}.elf PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++11 -fpermissive>)
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INC})
target_link_libraries(${PROJECT_NAME}.elf cross2dui ${LDFLAGS})
add_dependencies(${PROJECT_NAME}.elf ${PROJECT_NAME}.data)

#####################
# PSP2 (vita) vpk
#####################
if (BUILD_PSP2)
    add_custom_target(${PROJECT_NAME}.vpk
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND rm -rf vpk && mkdir -p vpk/sce_sys
            COMMAND ${VITASDK}/bin/vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
            COMMAND ${VITASDK}/bin/vita-make-fself -s -c ${PROJECT_NAME}.velf vpk/eboot.bin
            COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID="VITAFBA01" "${PROJECT_NAME}" vpk/sce_sys/param.sfo
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/pfba/data/skin vpk/
            COMMAND cp -r ${CMAKE_SOURCE_DIR}/pfba/data/psp2/* vpk
            COMMAND cd vpk && zip -r ../${PROJECT_NAME}-${PFBA_VERSION_MAJOR}.${PFBA_VERSION_MINOR}_vita.vpk . && cd ..
            )
endif (BUILD_PSP2)

#####################
# PS3
#####################
if (BUILD_PS3)
    add_custom_target(${PROJECT_NAME}.ps3
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND ${PSL1GHT}/bin/sprxlinker ${PROJECT_NAME}.elf
            COMMAND ${PSL1GHT}/bin/make_self ${PROJECT_NAME}.elf ${PROJECT_NAME}.ps3
            )
endif (BUILD_PS3)

#####################
# CTR (3DS) target
#####################
if (BUILD_3DS)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=3dsx.specs")
    add_3dsx_target(${PROJECT_NAME}.3ds "pFBA" "FBA 3DS" "Cpasjuste" pfba/data/3ds/icon.png)
    #add_cia_target(${PROJECT_NAME}.3ds pfba/data/3ds/app.rsf pfba/data/3ds/banner.png pfba/data/3ds/audio.wav "pFBA" "FBA 3DS" "Cpasjuste" pfba/data/3ds/icon.png)
endif (BUILD_3DS)

#####################
# NX (SWITCH) target
#####################
if (BUILD_SWITCH)
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS "-specs=${DEVKITPRO}/libnx/switch.specs")
    add_custom_target(${PROJECT_NAME}.nro
            DEPENDS ${PROJECT_NAME}.elf
            COMMAND nacptool --create "pFBA" "Cpasjuste" "${PFBA_VERSION_MAJOR}.${PFBA_VERSION_MINOR}" pfba.nacp
            COMMAND elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${CMAKE_CURRENT_SOURCE_DIR}/pfba/data/nx/icon.jpg --nacp=pfba.nacp
            )
    add_custom_target(${PROJECT_NAME}_release_switch.zip
            DEPENDS ${PROJECT_NAME}.nro
            COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/pfba
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pfba
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pfba/roms
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pfba/hiscores
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pfba/titles
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pfba/previews
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/pfba/icons
            COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/pfba.nro ${CMAKE_BINARY_DIR}/release/pfba/pfba.nro
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pfba/data/skin ${CMAKE_BINARY_DIR}/release/pfba/
            COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/pfba/data/nx/skin ${CMAKE_BINARY_DIR}/release/pfba/
            COMMAND cp -f ${CMAKE_CURRENT_SOURCE_DIR}/pfba/data/hiscore.dat ${CMAKE_BINARY_DIR}/release/pfba/hiscores
            COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}-${PFBA_VERSION_MAJOR}.${PFBA_VERSION_MINOR}_switch.zip pfba && cd ${CMAKE_BINARY_DIR}
            )
endif (BUILD_SWITCH)
