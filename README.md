# SuperBasicMusicPlayer

<img src="sbmp.png" alt="Super Basic Music Player screen, playing 'Light it Up', a song by Robin Hustin, TobiMorrow and Jex, released by NCS." />

A WIP music player for Linux (originally launched via terminal). This code was initially made in few hours out of boredom while I was without internet connection, purely based on other projects I have made. I added and changed some things later on.

## How does it work?

You must pass a song file through command line or dragging and dropping the file. It can play Vorbis (.ogg), MP3 and FLAC.

## Issues (to be fixed)

Unclean code. Untested using libraries already on the system. Cannot play Opus and other music formats. Wasn't made to run on Windows nor MacOS X (might run if you fix the dependencies, but no guarantees). Might work on Windows if you use MSYS2/MinGW64 to build it.

## Dependencies

SDL2 version 2.24.0; SDL2_ttf version 2.20.1; SDL_mixer version 2.6.2; SDL_ttf requires freetype.
If your system doesn't have any of this (such as some Linux distributions that doesn't update often), please build SuperBasicMusicPlayer using shipped static libraries (freetype still required).

## How can I build it?

Build using Premake (at least version 4, untested on Premake 5). There are 4 build options (build and release, using static libraries or installed ones).
You should generate GNU Makefiles, then you can use `make help` to show build options. Other generators might not work due to linkoptions using GCC parameters.

```
premake4 gmake
make config=releasestatic

```

## License

This project is under GNU GPL v3-or-later. Shipped libraries (SDL and related) are under Zlib license. The font is part of GNU FreeFont project and is under GNU GPL 3-or-later with Font-exception2.0
