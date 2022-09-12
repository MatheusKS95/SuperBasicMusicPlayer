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
