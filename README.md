# pemu

**Building - Windows (x64) => Windows (x64)**
- download, install and update [MYSYS2](http://www.msys2.org/)
- install msys2 dependencies:  
	- `pacman -S git zip automake autoconf make mingw-w64-x86_64-make mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-SDL2 mingw-w64-x86_64-freetype mingw-w64-x86_64-cmake mingw-w64-x86_64-glm mingw-w64-x86_64-glew mingw-w64-x86_64-libconfig mingw-w64-x86_64-tinyxml2 mingw-w64-x86_64-zlib mingw-w64-x86_64-perl mingw-w64-x86_64-ffmpeg mingw-w64-x86_64-mpv`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -G "MSYS Makefiles" -DPLATFORM_WINDOWS=ON -DOPTION_BUILTIN_MINIZIP=ON -DCMAKE_BUILD_TYPE=Release`
	- `make pfba.deps`
	- `make`

**Building - Ubuntu (x64) => Ubuntu (x64)**
- install dependencies:
	- `sudo apt-get install git zip build-essential cmake liblzma-dev libsdl2-dev libconfig-dev libcurl4-openssl-dev libtinyxml2-dev libconfig-dev libglm-dev libfreetype6-dev libpng-dev libminizip-dev zlib1g-dev libvdpau-dev libva-dev libavcodec-dev libavfilter-dev libavformat-dev libavresample-dev libavutil-dev libpostproc-dev libswresample-dev libswscale-dev`
- install recent mpv library from ppa:
    - `sudo add-apt-repository -y ppa:mc3man/bionic-media`
    - `sudo apt-get -y install libmpv-dev`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -DPLATFORM_LINUX=ON -DCMAKE_BUILD_TYPE=Release`
	- `make pfba.deps`
	- `make`

**Building - Ubuntu (x64) => Nintendo Switch**
- install ubuntu build tools:
	- `sudo apt-get install zip git autoconf libtool automake build-essential cmake`
- download, install and update devkitpro-pacman:
	- `wget https://github.com/devkitPro/pacman/releases/download/v1.0.2/devkitpro-pacman.amd64.deb && sudo dpkg -i devkitpro-pacman.amd64.deb`
	- `sudo dkp-pacman -Syu`
	- `sudo dkp-pacman -S switch-dev switch-glm switch-portlibs devkitpro-pkgbuild-helpers`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `cd pemu`
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -DPLATFORM_SWITCH=ON -DCMAKE_BUILD_TYPE=Release`
	- `make pfba.deps`
	- `make -j9 pfba_switch_release`
