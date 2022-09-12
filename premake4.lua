--	Super Basic Music Player
--	Copyright (C) 2022  Matheus K. Schaefer
--
--	This program is free software: you can redistribute it and/or modify
--	it under the terms of the GNU General Public License as published by
--	the Free Software Foundation, either version 3 of the License, or
--	(at your option) any later version.
--
--	This program is distributed in the hope that it will be useful,
--	but WITHOUT ANY WARRANTY; without even the implied warranty of
--	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--	GNU General Public License for more details.

--	You should have received a copy of the GNU General Public License
--	along with this program.  If not, see <https://www.gnu.org/licenses/>.

solution "SuperBasicMusicPlayer"
	configurations { "DebugStatic", "DebugSystem", "ReleaseStatic", "ReleaseSystem" }

	project "SuperBasicMusicPlayer"
		kind "WindowedApp"
		language "C"

		files { "main.c" }

		--build using debug symbols and shipped static libraries
		configuration "DebugStatic"
			defines { "DEBUG" }
			defines { "STATIC" }
			flags { "Symbols" }
			includedirs { "external/include" }
			linkoptions { "-Bstatic external/libs/libSDL2.a external/libs/libSDL2_mixer.a external/libs/libSDL2_ttf.a -Bdynamic" }
			links { "m", "pthread", "freetype", "dl" }

		--build using debug symbols and system libraries
		configuration "DebugSystem"
			defines { "DEBUG" }
			flags { "Symbols" }
			links { "m", "SDL2", "SDL2_mixer", "SDL2_ttf", "pthread", "freetype", "dl" }

		--build without using debug symbols (release) and using shppped static libraries
		configuration "ReleaseStatic"
			defines { "NDEBUG" }
			defines { "STATIC" }
			flags { "Optimize" }
			includedirs { "external/include" }
			linkoptions { "-Bstatic external/libs/libSDL2.a external/libs/libSDL2_mixer.a external/libs/libSDL2_ttf.a -Bdynamic" }
			links { "m", "pthread", "freetype", "dl" }

		--build without using debug symbols (release) and using system libraries
		configuration "ReleaseStatic"
			defines { "NDEBUG" }
			flags { "Optimize" }
			links { "m", "SDL2", "SDL2_mixer", "SDL2_ttf", "pthread", "freetype", "dl" }
