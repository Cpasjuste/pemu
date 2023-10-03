pGEN: Portable Genesis
===========================

![](https://github.com/Cpasjuste/pemu/raw/master/src/cores/pgen/data/screenshot.png)

### The main goals of this [Genesis-Plus-GX](https://github.com/ekeeke/Genesis-Plus-GX) port are:
- have a basic but user-friendly user interface on non Windows devices
- use a recent rendering interface for speed and effects support
- cross-platform and code portability
- easy updates of genesis-plus-gx core (no modifications to the original sources)

### Currently supported devices
- Linux
- Nintendo Switch
- Sony PS4
- Sony Ps Vita

### Capabilities
- Support save states
- Support 2x, 3x, fit, fit 4/3 and fullscreen hardware scaling
- Support hardware filtering (shaders)

### Installation (switch)
- copy "pgen.nro" directory to "/switch/pgen/" directory on sdcard
- copy gamegear roms to "/switch/pgen/gamegear/" directory on sdcard
- copy megadrive roms to "/switch/pgen/megadrive/" directory on sdcard
- copy master system roms to "/switch/pgen/mastersystem/" directory on sdcard
- copy sg-1000 roms to "/switch/pgen/sg1000/" directory on sdcard
- copy mega-cd roms (unzipped cue/bin) to "/switch/pgen/megacd/" directory on sdcard

### Installation (ps4)
- install pkg
- copy gamegear roms to "/data/pgen/gamegear/" directory on sdcard
- copy megadrive roms to "/data/pgen/megadrive/" directory on sdcard
- copy master system roms to "/data/pgen/mastersystem/" directory on sdcard
- copy sg-1000 roms to "/data/pgen/sg1000/" directory on sdcard
- copy mega-cd roms (unzipped cue/bin) to "/data/pgen/megacd/" directory on sdcard

### Installation (vita)
- install vpk
- copy gamegear roms to "ux0:/data/pgen/gamegear/" directory on sdcard
- copy megadrive roms to "ux0:/data/pgen/megadrive/" directory on sdcard
- copy master system roms to "ux0:/data/pgen/mastersystem/" directory on sdcard
- copy sg-1000 roms to "ux0:/data/pgen/sg1000/" directory on sdcard
- copy mega-cd roms (unzipped cue/bin) to "ux0:/data/pgen/megacd/" directory on sdcard

### Bios
You need to put sega bios in the "pgen/bios" directory:
ggenie.bin, areplay.bin, sk.bin, sk2chip.bin, bios_CD_U.bin, bios_CD_E.bin, bios_CD_J.bin, bios_MD.bin, bios_U.sms, bios_E.sms, bios_J.sms, bios.gg

### Usage (switch)
- Press (L) in the rom list to configure main options and default roms options
- Press (R) in the rom list to configure per rom options
- Press (L) AND (R) when in emulation to bring the options menu

### Thanks
- ScHlAuChi
- devkitPro and libnx people: yellows8, plutoo, profi200, WntrMute ....
- ScHlAuChi again :)
