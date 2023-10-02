pGBA: Portable GameBoy Advance
===========================

![](https://github.com/Cpasjuste/pemu/raw/master/pgba/data/screenshot.png)

### The main goals of this [mGBA](https://github.com/mgba-emu/mgba) port are:

- have a basic but user-friendly user interface on non Windows devices
- use a recent rendering interface for speed and effects support
- cross-platform and code portability
- easy updates of mGBA core (no modifications to the original sources)

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

### Bios

You need to put gba bios "gba_bios.bin"" in the "bios" directory

### Usage (switch)

- Press (L) in the rom list to configure main options and default roms options
- Press (R) in the rom list to configure per rom options
- Press (L) AND (R) when in emulation to bring the options menu

### Thanks

- ScHlAuChi
- devkitPro and libnx people: yellows8, plutoo, profi200, WntrMute ....
- ScHlAuChi again :)
