pNES: Portable Snes9x
===========================

![](https://github.com/Cpasjuste/pemu/raw/master/pnes/pnes/data/screenshot.png)


### The main goals of this [nestopia](https://github.com/0ldsk00l/nestopia) port are:

- have a basic but user friendly user interface on non windows devices
- use a recent rendering interface for speed and effects support
- cross-platform and code portability
- easy updates of nestopia core (no modifications to the original sources)

### Currently supported devices

- Linux (sdl1, sdl2)
- Nintendo Switch (libnx, sdl2)
- Sony Ps Vita (vitasdk, vita2d)

### Capabilities
- Support save states
- Support cheats (put a "rom_name.cht"  file with game genie codes in cheat folder)
- Support 2x, 3x, fit, fit 4/3 and fullscreen hardware linear scaling
- Support hardware filtering (shaders)
- Support titles, previews, and icons images (png), named as the rom name with png extension instead zip/sfc, in their respective directory (to be created if needed).

### Installation (switch)
- copy "pnes" directory to "/switch/" directory on sdcard
- copy roms to "/switch/pnes/roms/" directory on sdcard

### Usage (switch)
- Press (+) in the rom list to configure main options and default roms options
- Press (-) in the rom list to configure per rom options
- Press (+) AND (-) when in emulation to bring the options menu

### Thanks
- ScHlAuChi
- devkitPro and libnx people: yellows8, plutoo, profi200, WntrMute ....
- ScHlAuChi again :)
