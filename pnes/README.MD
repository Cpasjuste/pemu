pNES: Portable NES
===========================

![](https://github.com/Cpasjuste/pemu/raw/master/pnes/data/screenshot.png)


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
- Support 2x, 3x, fit, fit 4/3 and fullscreen hardware scaling
- Support hardware filtering (shaders)
- Support shaders effects

### Romset
- The pNES database is built against the no-intro romset "No-Intro - Nintendo - Nintendo Entertainment System (2017-06-18).zip".

### Preview images and videos
- pNES support preview images and videos (put files in roms media folder: png files goes in "roms/media/mixrbv2", mp4 files in "roms/media/video"). Please note that the vita port doesn't support video previews.
You can download a near fullset mixrbv2 and video package (either in SD or HD format) here:
    - preview images: [mixrbv2 (141 MB)](http://files.mydedibox.fr/files/dev/pemu/pnes/pnes_mixrbv2.zip), [mixrbv2 HD (900 MB)](http://files.mydedibox.fr/files/dev/pemu/pnes/pnes_mixrbv2_hd.zip)
    - preview videos: [video (1372 MB)](http://files.mydedibox.fr/files/dev/pemu/pnes/pnes_video.zip), [video HD (5822 MB)](http://files.mydedibox.fr/files/dev/pemu/pnes/pnes_video_hd.zip)

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
