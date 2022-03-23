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
- The pGEN database (for infos and medias) is built against the no-intro romset "No-Intro - Sega - Mega Drive - Genesis (2017-03-18).zip".

### Preview images and videos
- pGEN support preview images and videos (put files in roms media folder: png files goes in "roms/media/mixrbv2", mp4 files in "roms/media/video"). Please note that the vita port doesn't support video previews.
You can download a near full-set mixrbv2 and video package (either in high quality or low quality format) here:
    - preview images: [mixrbv2 (TODO)](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megadrive_mixrbv2.zip) (XX MB)
    - preview videos: [video_hq (TODO)](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megadrive_video_hq.zip) (XX GB), [video_lq (TODO)](http://files.mydedibox.fr/files/dev/pemu/pgen/pgen_megadrive_video_lq.zip) (XX GB)

### Installation (switch)
- copy "pgen.nro" directory to "/switch/pgen/" directory on sdcard
- copy roms to "/switch/pgen/roms/" directory on sdcard

### Installation (ps4)
- install pkg
- copy roms to "/data/pgen/roms/" directory on hdd

### Installation (vita)
- install vpk
- copy roms to "ux0:/data/pgen/roms/" directory on hdd

### Usage (switch)
- Press (L) in the rom list to configure main options and default roms options
- Press (R) in the rom list to configure per rom options
- Press (L) AND (R) when in emulation to bring the options menu

### Thanks
- ScHlAuChi
- devkitPro and libnx people: yellows8, plutoo, profi200, WntrMute ....
- ScHlAuChi again :)
