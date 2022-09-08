pGEN: Portable Genesis
===========================

![](https://github.com/Cpasjuste/pemu/raw/master/pgen/data/screenshot.png)

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

### Romset
- The pGEN database (for infos and medias) is built against these no-intro sets:
  - No-Intro - Sega - Mega Drive - Genesis (2017-03-18).zip
  - No-Intro - Sega - Game Gear (20210519).zip
  - No-Intro - Sega - Master System - Mark III (2017-04-14).zip
  - [REDUMP] Disc Image Collection: Sega - Sega Mega CD updated 2021-01-08 (20-42-03)

### Preview images and videos
- pGEN support preview images and videos (put files in roms media folder: png files goes in "roms/media/mixrbv2", mp4 files in "roms/media/video", replace "roms" with the system roms path). Please note that the vita port doesn't support video previews.
You can download full-set mixrbv2 and video packages here:
    - gamegear preview images: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_gamegear_mixrbv2.zip) (181 MB)
    - gamegear preview videos: [video](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_gamegear_video.zip) (1.9 GB)
    - megadrive preview images: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megadrive_mixrbv2.zip) (807 MB)
    - megadrive preview videos: [video](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megadrive_video.zip) (9 GB)
    - master system preview images: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_sms_mixrbv2.zip) (220 MB)
    - master system preview videos: [video](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_sms_video.zip) (2.2 GB)
    - mega-cd preview images: [mixrbv2](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megacd_mixrbv2.zip) (174 MB)
    - mega-cd preview videos: [video](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megacd_video.zip) (848 MB)

### Installation (switch)
- copy "pgen.nro" directory to "/switch/pgen/" directory on sdcard
- copy gamegear roms to "/switch/pgen/gamegear/" directory on sdcard
- copy megadrive roms to "/switch/pgen/megadrive/" directory on sdcard
- copy master system roms to "/switch/pgen/sms/" directory on sdcard
- copy mega-cd roms (unzipped cue/bin) to "/switch/pgen/megacd/" directory on sdcard

### Installation (ps4)
- install pkg
- copy gamegear roms to "/data/pgen/gamegear/" directory on sdcard
- copy megadrive roms to "/data/pgen/megadrive/" directory on sdcard
- copy master system roms to "/data/pgen/sms/" directory on sdcard
- copy mega-cd roms (unzipped cue/bin) to "/data/pgen/megacd/" directory on sdcard

### Installation (vita)
- install vpk
- copy gamegear roms to "ux0:/data/pgen/gamegear/" directory on sdcard
- copy megadrive roms to "ux0:/data/pgen/megadrive/" directory on sdcard
- copy master system roms to "ux0:/data/pgen/sms/" directory on sdcard
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
