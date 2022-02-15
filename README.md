# SuperBasicMusicPlayer

A WIP music player for Linux terminal. This code was made in few hours out of boredom while I was without internet connection, purely based on other projects I have made.

## How does it work?

You must pass a song file through command line. It can play Vorbis, MP3 and FLAC. However MP3 and FLAC are buggy (the song plays but no metadata shown correctly - or at all).

## Issues (to be fixed)

MP3 and FLAC doesn't show metadata. If Vorbis metadata are incomplete (title, album name and artist are required) you will be greeted with a beautiful segmentation fault. Probably there's some buffer overflow somewhere. Font file are required in the build directory. There are a lot of char array handlings there. Pretty scary code.

## Dependencies

SDL 2, SDL_Image (to be removed - unused for now) and SDL_Mixer. stb_vorbis already included with the code.

## How can I build it?

Build using CMake. You need to copy the ttf font to the build directory in order to work otherwise you'll be greeted with an error.
