pFBA: Portable Final Burn Alpha
===============================

![](https://github.com/Cpasjuste/pfba/raw/master/pfba/data/screenshot.png)

-----

#### The main goals of this [final burn alpha](http://www.fbalpha.com/) port are :

- have a basic but user friendly user interface on non windows devices
- use a recent rendering interface for speed and effect support
- cross-platform and code portability
- easy updates of fba core (no modifications to the original sources)
- use arm asm m68k ([cyclone](https://github.com/notaz/cyclone68000)) core on arm devices

-----

#### Supported devices

- Linux (sdl1, sdl2)
- Sony Ps Vita (vita2d)
- Nintendo Switch (libnx, sdl2)

-----

#### GENERAL INFORMATION'S

- pFBA support console roms (Sega MegaDrive, Sega GameGear, Sega Master System, Nec PC-Engine, Nec TurboGraFX-16, Nec SuperGraFX, Coleco, SG-1000 and MSX). Here is the corresponding folders where roms should be located (as stock fba):
    ```
    Sega MegaDrive      : "megadriv"
    Sega GameGear       : "gamegear"
    Sega Master System  : "sms"
    Nec PC-Engine       : "pce"
    Nec TurboGraFX-16   : "tg16"
    Nec SuperGraFX      : "sgx"
    Coleco              : "coleco"
    SG-1000             : "sg1000"
    MSX                 : "msx"
    ```
- pFBA support title, preview, and icon images in "png" format. Titles goes in the "titles" folder, previews in the "previews" folder, and icons in the "icons" folder. They must have the same filename as the zipped rom, but with a ".png" extension instead ".zip". For consoles images, you need to add the console driver prefix to the image filename ("sonic.zip" image for Sega MegaDrive would be "md_sonic.png").
- You can enable or disable icon images from the pFBA main menu. A restart is required to enable the changes.

-----

### NINTENDO SWITCH INFORMATION'S

* ##### GENERAL INFORMATION'S
    * Using icons on the Nintendo Switch will make main loading time longer if a lot of icons are detected. There's currently no plan to improve this.
    * You can enable or disable single joycon support from the pFBA main menu (+). When enabled, all detected joycons (not docked) will be handled as a single controller/player.
    * Point scaling is currently limited to 1x, 2x and 3x size. FIT, FIT 4/3 and FULL options will fallback to 3x.

* ##### NINTENDO SWITCH CONTROLS (UI)
   ```
   (L/R)            : SWITCH FROM TITLE TO PREVIEW IMAGE
   (+)              : GENERAL OPTIONS + DEFAULT ROMS OPTIONS
   (-)              : PER ROM OPTIONS
   (A)              : RUN ROM
   ```

* ##### NINTENDO SWITCH CONTROLS (IN-GAME)
   ```
   (+)              : START
   (-)              : COINS
   (+/-) + (A)      : START
   (+/-) + (B)      : COINS
   (+/-) + (X)      : SERVICE SWITCH
   (+/-) + (Y)      : 1P + 2P START
   (+/-) + (L/R)    : PFBA MENU
   (+)   + (-)      : UNIBIOS MENU (NEOGEO ONLY)
   ```

-----

### PS VITA INFORMATION'S (TO BE UPDATED)

- data is located at "ux0:/data/pfba"
- default rom path is "ux0:/data/pfba/roms/"
- rom path can be changed in "ux0:/data/pfba/pfba.cfg"
- preview images can be put in "ux0:/data/pfba/previews/rom-name.png"
- ui: START = general options + default roms configuration
- ui: SELECT = per rom configuration
- in game: SELECT+START = menu
- in game: SELECT+L = saves states
- in game: SELECT+SQUARE = service switch
- in game: SELECT+TRIANGLE = 1P+2P switch
- in game: SELECT+LEFT/RIGHT = change shader
- in game: SELECT+UP/DOWN = change scaling

----

### COMPILATION

* ##### LINUX (SDL2)
   ```
   sudo apt-get install libsdl2-dev libfreetype6-dev libpng-dev zlib1g-dev
   git clone --recursive https://github.com/Cpasjuste/pfba.git
   cd pfba && git submodule update --init --recursive
   mkdir cmake-build
   cd cmake-build
   cmake -DBUILD_SDL2=ON -DBUILD_SDL2_GL=ON -DCMAKE_BUILD_TYPE=Release ..
   make pfba.deps
   make pfba.elf
   ```

* ##### NINTENDO SWITCH
   ```
   git clone --recursive https://github.com/Cpasjuste/pfba.git
   cd pfba && git submodule update --init --recursive
   mkdir cmake-build
   cd cmake-build
   cmake -DBUILD_NX=ON -DCMAKE_BUILD_TYPE=Release ..
   make pfba.deps
   make pfba_release_switch.zip
   ```

* ##### PS VITA
   ```
   git clone --recursive https://github.com/Cpasjuste/pfba.git
   cd pfba && git submodule update --init --recursive
   mkdir cmake-build
   cd cmake-build
   cmake -DBUILD_PSP2=ON -DCMAKE_BUILD_TYPE=Release ..
   make pfba.deps
   make pfba.vpk
   ```

-----

### Thanks
- Xavier for the support (http://www.consolex-bordeaux.fr/)
- ScHlAuChi for all the advices and testing
- fba/mame authors (http://www.fbalpha.com/, http://mamedev.org/)
- xerpi for vita2d (https://github.com/xerpi/libvita2d)
- frangarcj for vita2d shader support (https://github.com/frangarcj/vita2dlib)
- people behind the vitasdk (https://github.com/vitasdk)
