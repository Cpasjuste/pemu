pFBN: Portable FinalBurn Neo
===============================

![](https://github.com/Cpasjuste/pemu/raw/master/pfba/data/screenshot.png)

-----

#### The main goals of this [fbneo](https://github.com/finalburnneo/FBNeo) port are:

- have a basic but user friendly user interface
- use a recent rendering interface for speed and effect support
- cross-platform and code portability
- easy updates of fbneo core (no modifications to the original sources)
- use arm asm m68k ([cyclone](https://github.com/notaz/cyclone68000)) core on arm devices

-----

#### Supported devices

- Linux
- Windows
- Sony Ps Vita
- Nintendo Switch

-----

#### GENERAL INFORMATION'S

- pFBN, as fbneo, support consoles roms. Here is the corresponding folders where roms should be located (as fbneo):
    ```
    Arcade              : "roms"
    Coleco              : "coleco"
    Sega GameGear       : "gamegear"
    Sega MegaDrive      : "megadriv"
    MSX                 : "msx"
    Nec PC-Engine       : "pce"
    SG-1000             : "sg1000"
    Nec SuperGraFX      : "sgx"
    Sega Master System  : "sms"
    Nec TurboGraFX-16   : "tg16"
    Spectrum            : "zx3"
    Nintendo NES        : "nes"
    Nintendo NES FDS    : "nes_fds"
    ```
  
- pFBN support preview images and videos (put files in corresponding media folder. For arcade roms, for example: png files goes in "roms/media/mixrbv2", mp4 files in "roms/media/video"). Please note that the vita port doesn't support video previews.
You can download a near fullset mixrbv2 and video package for each platform here:
    - Arcade: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/arcade_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/arcade_video.zip)
    - Coleco: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/coleco_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/coleco_video.zip)
    - Sega GameGear: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/gamegear_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/gamegear_video.zip)
    - Sega MegaDrive: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/megadriv_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/megadriv_video.zip)
    - MSX: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/msx_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/msx_video.zip)
    - Nec PC-Engine: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/pce_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/pce_video.zip)
    - SG-1000: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sg1000_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sg1000_video.zip)
    - Nec SuperGraFX: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sgx_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sgx_video.zip)
    - Sega Master System: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sms_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sms_video.zip)
    - Nec TurboGraFX-16: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/tg16_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/tg16_video.zip)
    - Spectrum: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/zx3_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/zx3_video.zip)
    - Nintendo NES: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/nes_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/nes_video.zip)
    - Nintendo NES FDS: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/nes_fds_mixrbv2.zip), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/nes_fds_video.zip)

-----

### NINTENDO SWITCH INFORMATION (may be outdated)

* ##### GENERAL INFORMATION
    * Using icons on the Nintendo Switch will make main loading time longer if a lot of icons are detected. There's currently no plan to improve this.
    * You can enable or disable single joycon support from the pFBN main menu (+). When enabled, all detected joycons (not docked) will be handled as a single controller/player.
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
   (+/-) + (L/R)    : pFBN MENU
   (+)   + (-)      : UNIBIOS MENU (NEOGEO ONLY)
   ```

-----

### PS VITA INFORMATION'S ((may be outdated))

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

- see pemu [README.MD](https://github.com/Cpasjuste/pemu)

-----

### Thanks
- Xavier for the support (http://www.consolex-bordeaux.fr/)
- ScHlAuChi for all the advices, testing, and hardware support
- fba/mame authors (http://www.fbalpha.com/, http://mamedev.org/)
- xerpi for vita2d (https://github.com/xerpi/libvita2d)
- frangarcj for vita2d shader support (https://github.com/frangarcj/vita2dlib)
- people behind the vitasdk (https://github.com/vitasdk)
- WinterMute and fincs, for their continue work on the Nintendo Switch
- [screencraper.fr](https://www.screenscraper.fr/)
