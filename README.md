# pemu

**Building - Windows (x64) => Windows (x64)**
- download, install and update [MYSYS2](http://www.msys2.org/)
- install msys2 dependencies:  
	- `pacman -S git zip automake autoconf make mingw-w64-x86_64-make mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-SDL2 mingw-w64-x86_64-freetype mingw-w64-x86_64-cmake mingw-w64-x86_64-glm mingw-w64-x86_64-glew mingw-w64-x86_64-libconfig mingw-w64-x86_64-json-c mingw-w64-x86_64-zlib mingw-w64-x86_64-perl`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -G "MSYS Makefiles" -DBUILD_WINDOWS=ON -DCMAKE_BUILD_TYPE=Release`

**Building - Ubuntu (x64) => Ubuntu (x64)**
- install dependencies:
	- `sudo apt-get install git zip build-essential cmake libsdl2-dev libconfig-dev libcurl4-openssl-dev libjson-c-dev libconfig-dev libglm-dev libfreetype6-dev libpng-dev libminizip-dev zlib1g-dev`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -DBUILD_LINUX=ON -DCMAKE_BUILD_TYPE=Release`
	
**Building - Ubuntu (x64) => Nintendo Switch**
- install ubuntu build tools:
	- `sudo apt-get install zip git autoconf libtool automake build-essential cmake`
- download, install and update devkitpro-pacman:
	- `wget https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb && sudo dpkg -i devkitpro-pacman.deb`
	- `sudo dkp-pacman -Syu`
	- `sudo dkp-pacman -S switch-dev switch-portlibs devkitpro-pkgbuild-helpers`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -DBUILD_SWITCH=ON -DCMAKE_BUILD_TYPE=Release`
	
