pFBN: Portable FinalBurn Neo
===============================

![](https://github.com/Cpasjuste/pemu/raw/master/pfbneo/data/screenshot.png)

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
- Sony PS4
- Nintendo Switch

-----

#### GENERAL INFORMATION'S

- pFBN current DATs files are located [here](https://github.com/libretro/FBNeo/tree/a9bce0459fdc5653842b5550542e8c6d0e5342f1/dats).

- pFBN, as fbneo, support consoles roms. Here is the corresponding folders where roms should be located (as fbneo):
    ```
    Arcade              : "roms"
    Channel F           : "channelf"
    Coleco              : "coleco"
    Nintendo NES FDS    : "fds"
    Sega GameGear       : "gamegear"
    Sega MegaDrive      : "megadrive"
    MSX                 : "msx"
    Nintendo NES        : "nes"
    Neo Geo Pocket      : "ngp"
    Nec PC-Engine       : "pce"
    SG-1000             : "sg1000"
    Nec SuperGraFX      : "sgx"
    Sega Master System  : "sms"
    Spectrum            : "spectrum"
    Nec TurboGraFX-16   : "tg16"
    ```
  
- pFBN support preview images and videos (put files in corresponding media folder. For arcade roms, for example: png files goes in "roms/media/mixrbv2", mp4 files in "roms/media/video"). Please note that the vita port doesn't support video previews.
You can download a near fullset mixrbv2 and video package for each platform here:
    - Arcade: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/arcade_mixrbv2.zip) (1.5GB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/arcade_video.zip) (7.1GB)
    - Channel F: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/channelf_mixrbv2.zip) (16MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/channelf_video.zip) (11MB)
    - Colecovision: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/coleco_mixrbv2.zip) (128MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/coleco_video.zip) (248MB)
    - MSX: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/msx_mixrbv2.zip) (77MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/msx_video.zip) (417MB)
    - Nec PC-Engine: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/pce_mixrbv2.zip) (200MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/pce_video.zip) (1.2GB)
    - Nec SuperGraFX: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sgx_mixrbv2.zip) (3MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sgx_video.zip) (60MB)
    - Nec TurboGraFX-16: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/tg16_mixrbv2.zip) (61MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/tg16_video.zip) (713MB)
    - Neo Geo Pocket: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/ngp_mixrbv2.zip) (47MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/ngp_video.zip) (280MB)
    - Nintendo NES: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/nes_mixrbv2.zip) (667MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/nes_video.zip) (5.3GB)
    - Nintendo NES FDS: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/fds_mixrbv2.zip) (71MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/fds_video.zip) (543MB)
    - Sega GameGear: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/gamegear_mixrbv2.zip) (178MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/gamegear_video.zip) (1.9GB)
    - Sega Master System: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sms_mixrbv2.zip) (219MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sms_video.zip) (2.2GB)
    - Sega MegaDrive: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/megadrive_mixrbv2.zip) (878MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/megadrive_video.zip) (9.5GB)
    - SG-1000: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sg1000_mixrbv2.zip) (49MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/sg1000_video.zip) (132MB)
    - Spectrum: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pfbneo/spectrum_mixrbv2.zip) (110MB), [video](http://files.mydedibox.fr/files/dev/pemu/pfbneo/spectrum_video.zip) (1.1GB)
-----

### NINTENDO SWITCH INFORMATION

* ##### NINTENDO SWITCH CONTROLS (UI)
|                                                                                                   Buttons                                                                                                   | Action                                 |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|----------------------------------------|
|                                                     <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/>                                                      | General options + default roms options |
|                                                     <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                      | Per rom options                        |
| <img align="center" src="./data/switch/romfs/skins/default/buttons/104.png" width="32" height="32"/> / <img align="center" src="./data/switch/romfs/skins/default/buttons/105.png" width="32" height="32"/> | Switch systems                         |

* ##### NINTENDO SWITCH CONTROLS (UI, SINGLE JOYCON MODE, 1P ONLY)
|                                                                                                   Buttons                                                                                                   | Action                                 |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|----------------------------------------|
|   <img align="center" src="./data/switch/romfs/skins/default/buttons/7.png" width="32" height="32"/> / <img align="center" src="./data/switch/romfs/skins/default/buttons/8.png" width="32" height="32"/>   | General options + default roms options |
|   <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/> / <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>   | Per rom options                        |
| <img align="center" src="./data/switch/romfs/skins/default/buttons/104.png" width="32" height="32"/> / <img align="center" src="./data/switch/romfs/skins/default/buttons/105.png" width="32" height="32"/> | Switch systems (SL/SR)                 |

* ##### NINTENDO SWITCH CONTROLS (IN-GAME)
|                                                                                                                                                                                                      Buttons                                                                                                                                                                                                      | Action                               |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|--------------------------------------|
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                                                                                                                         | Coins                                |
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/>                                                                                                                                                         | Start                                |
|                                                                                                      <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                                                                      | pFBN menu                            |
| <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/2.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/0.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/1.png" width="32" height="32"/> | UniBios menu (Neo-Geo)               |
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/>                                                                                                                                                         | Hold 2 seconds for diagnostic menu   |
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                                                                                                                         | Hold 2 seconds to reset current game |

* ##### NINTENDO SWITCH CONTROLS (IN-GAME, SINGLE JOYCON MODE, LEFT)
|                                                                                                                                                                                                       Buttons                                                                                                                                                                                                        | Action                                         |
|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|------------------------------------------------|
|                                                                                                                                                          <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                                                                                                                          | Coins                                          |
|                                                                                                                                                          <img align="center" src="./data/switch/romfs/skins/default/buttons/7.png" width="32" height="32"/>                                                                                                                                                          | Start                                          |
|                                                                                                       <img align="center" src="./data/switch/romfs/skins/default/buttons/7.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                                                                        | pFBN menu (1P only)                            |
| <img align="center" src="./data/switch/romfs/skins/default/buttons/7.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/13.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/12.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/14.png" width="32" height="32"/> | UniBios menu (Neo-Geo) (1P only)               |
|                                                                                                                                                          <img align="center" src="./data/switch/romfs/skins/default/buttons/7.png" width="32" height="32"/>                                                                                                                                                          | Hold 2 seconds for diagnostic menu (1P only)   |
|                                                                                                                                                          <img align="center" src="./data/switch/romfs/skins/default/buttons/4.png" width="32" height="32"/>                                                                                                                                                          | Hold 2 seconds to reset current game (1P only) |

* ##### NINTENDO SWITCH CONTROLS (IN-GAME, SINGLE JOYCON MODE, RIGHT)
|                                                                                                                                                                                                      Buttons                                                                                                                                                                                                      | Action                                         |
|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|------------------------------------------------|
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/>                                                                                                                                                         | Coins                                          |
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/8.png" width="32" height="32"/>                                                                                                                                                         | Start                                          |
|                                                                                                      <img align="center" src="./data/switch/romfs/skins/default/buttons/8.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/>                                                                                                      | pFBN menu (1P only)                            |
| <img align="center" src="./data/switch/romfs/skins/default/buttons/8.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/0.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/1.png" width="32" height="32"/> + <img align="center" src="./data/switch/romfs/skins/default/buttons/3.png" width="32" height="32"/> | UniBios menu (Neo-Geo) (1P only)               |
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/8.png" width="32" height="32"/>                                                                                                                                                         | Hold 2 seconds for diagnostic menu (1P only)   |
|                                                                                                                                                        <img align="center" src="./data/switch/romfs/skins/default/buttons/6.png" width="32" height="32"/>                                                                                                                                                         | Hold 2 seconds to reset current game (1P only) |
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
