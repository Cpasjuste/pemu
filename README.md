# pemu

**Building - Windows (x64)**
- download, install and update [MYSYS2](http://www.msys2.org/)
- install dependencies:  
	- `pacman -S git zip automake autoconf make mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2 mingw-w64-x86_64-freetype 
	mingw-w64-x86_64-cmake mingw-w64-x86_64-glm mingw-w64-x86_64-glew mingw-w64-x86_64-libconfig mingw-w64-x86_64-json-c mingw-w64-x86_64-zlib`
- clone pemu repository:
	- `git clone --recursive https://github.com/Cpasjuste/pemu.git`
- build:
	- `mkdir cmake-build && cd cmake-build`
	- `cmake ../ -G "MSYS Makefiles" -DBUILD_WINDOWS=ON`