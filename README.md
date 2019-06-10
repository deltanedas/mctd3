# mctd3
Requires SDL2, SDL2_image, SDL2_ttf, SDL2_mixer.
On Debian and its derivatives:
  `sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0 libsdl2-mixer-2.0.0`

An official unofficial sequel to the legendary flash game Minecraft Tower Defense 2. This version is based on SDL2, rather than flash.

If you encounter any bugs please create an issue in the Issues tab.
Due to copyright restrictions you'll have to supply your own block textures. Current version is just a tech demo of SimpleUI.
You can use your own textures with the Packer/Unpacker programs in Packing. 

# Building

**Dependencies**
g++-8, SDL2, SDL2_image, SDL2_ttf, [SDL2_fontcache \(C++ version\)](https://github.com/deltanedas/SDL_fontcache/), [LuaBridge](https://github.com/vinniefalco/LuaBridge) pthread

**On \*NIX**
1. `make run`

**On Windows (EXPERIMENTAL)**
1. Get [MinGW](https://mingw-w64.org/) and [make](http://gnuwin32.sourceforge.net/packages/make.htm)
2. `make`
3. Run `MCTD3`
(probably doesn't work and/or needs makefile tinkering)
