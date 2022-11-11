/*
    Super Basic Music Player
    Copyright (C) 2022  Matheus K. Schaefer

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#ifdef STATIC
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif

const int WIDTH = 600;
const int HEIGHT = 300;

SDL_Window *window;
Mix_Music *music;

SDL_Surface *music_title_playing;
SDL_Surface *music_artist_playing;
SDL_Surface *music_album_playing;
SDL_Surface *music_title_empty;
SDL_Surface *music_artist_empty;
SDL_Surface *music_album_empty;

TTF_Font *font_title;
TTF_Font *font_others;

SDL_Color color;

void loadmusic(char *filename)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	music = Mix_LoadMUS(filename);
	if(music == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "F A I L %s", Mix_GetError());
	}

	Mix_PlayMusic(music, 0);
}

void loadmetadata()
{
	music_title_playing = TTF_RenderUTF8_Blended(font_title, strlen(Mix_GetMusicTitleTag(music)) != 0 ? Mix_GetMusicTitleTag(music) : "No title", color);
	SDL_Rect rect_title_p = music_title_playing->clip_rect;
	rect_title_p.x = 20;
	rect_title_p.y = 20;
	music_artist_playing = TTF_RenderUTF8_Blended(font_others, strlen(Mix_GetMusicArtistTag(music)) != 0 ? Mix_GetMusicArtistTag(music) : "No artist info", color);
	SDL_Rect rect_artist_p = music_artist_playing->clip_rect;
	rect_artist_p.x = 20;
	rect_artist_p.y = rect_title_p.y + 70;
	music_album_playing = TTF_RenderUTF8_Blended(font_others, strlen(Mix_GetMusicAlbumTag(music)) != 0 ? Mix_GetMusicAlbumTag(music) : "No album info", color);
	SDL_Rect rect_album_p = music_album_playing->clip_rect;
	rect_album_p.x = 20;
	rect_album_p.y = rect_artist_p.y + 30;

	char window_title[51];
	snprintf(window_title, 50, "%s - SUPER BASIC MUSIC PLAYER", Mix_GetMusicTitle(music));
	window_title[50] = '\0';

	SDL_SetWindowTitle(window, window_title);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    SDL_Surface *surface;

    window = NULL;
    surface = NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Quit();
        return -1;
    }
	printf("SuperBasicMusicPlayer  Copyright (C) 2022  Matheus K. Schaefer\n");
	printf("This is free software, and you are welcome to redistribute it under the terms of GNU GPL v3 or later.\n");
	printf("This software also includes SDL code (including SDL_mixer and SDL_ttf, which is licensed under Zlib license. Copyright (C) 1997-2022 Sam Lantinga <slouken@libsdl.org>\n");
	
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
    
	#ifdef DEBUG
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Running in debug mode");
	#else
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Running in release mode");
    #endif
	
	#ifdef STATIC
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using shipped static libraries");
	#else
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using installed libraries. Project was compiled using SDL version %u.%u.%u and linked against %u.%u.%u", compiled.major, compiled.minor, compiled.patch, linked.major, linked.minor, linked.patch);
	#endif

    window = SDL_CreateWindow("SUPER BASIC MEDIA PLAYER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    if(window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    int soundflags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC;
    int initsnd = Mix_Init(soundflags);
	if((initsnd&soundflags) != soundflags)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", Mix_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to init SDL Mixer", window);
        SDL_Quit();
        return -1;
    }

    if(TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", TTF_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to init SDL TTF", window);
        SDL_Quit();
        return -1;
    }

    surface = SDL_GetWindowSurface(window);

    SDL_Surface *icon_playpause;
    SDL_Surface *icon_stop;
    SDL_Surface *icon_web;

    font_title = TTF_OpenFont("FreeSansOblique.ttf", 48);
    font_others = TTF_OpenFont("FreeSansOblique.ttf", 24);

    color.r = 255;
    color.g = 255;
    color.b = 0;
    color.a = 255;
    music_title_empty = TTF_RenderUTF8_Blended(font_title, "No music", color);
    SDL_Rect rect_title = music_title_empty->clip_rect;
    rect_title.x = 20;
    rect_title.y = 20;

    music_artist_empty = TTF_RenderUTF8_Blended(font_others, "--", color);
    SDL_Rect rect_artist = music_artist_empty->clip_rect;
    rect_artist.x =  20;
    rect_artist.y = rect_title.y + 70;

    music_album_empty = TTF_RenderUTF8_Blended(font_others, "--", color);
    SDL_Rect rect_album = music_album_empty->clip_rect;
    rect_album.x =  20;
    rect_album.y = rect_artist.y + 30;

    icon_playpause = TTF_RenderText_Blended(font_others, "PLAY/PAUSE", color);
    SDL_Rect rect_button_playpause = icon_playpause->clip_rect;
    //rect_button_playpause.h = 75;
    //rect_button_playpause.w = 75;
    rect_button_playpause.x = 40;
    rect_button_playpause.y = HEIGHT - 105;
    icon_stop = TTF_RenderText_Blended(font_others, "STOP", color);
    SDL_Rect rect_button_stop = icon_stop->clip_rect;
    //rect_button_stop.h = 45;
    //rect_button_stop.w = 45;
    rect_button_stop.x = rect_button_playpause.x + rect_button_playpause.w + 50;
    rect_button_stop.y = HEIGHT - 105;
    icon_web = TTF_RenderText_Blended(font_others, "Search artist", color);
    SDL_Rect rect_button_web = icon_web->clip_rect;
    rect_button_web.x = rect_button_stop.x + rect_button_stop.w + 50;
    rect_button_web.y = HEIGHT - 105;

    SDL_Event event;
    int playing = 0;
    int playpause_selected = 0;
    int playpause_pressed = 0;
    int stop_selected = 0;
    int stop_pressed = 0;

    //check if there are any arguments
    if(argc >= 2)
    {
		loadmusic(argv[1]);
		
		loadmetadata();
    }
    else
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning", "There's no music to play - please drag and drop one.", window);
    }

    while(playing == 0)
    {
        SDL_UpdateWindowSurface(window);
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
				const SDL_Point pt = {event.button.x, event.button.y};
				if(SDL_PointInRect(&pt, &rect_button_playpause))
				{
					playpause_pressed = 1;
					if(!Mix_PausedMusic())
					{
						Mix_PauseMusic();
					}
					else
					{
						Mix_ResumeMusic();
					}

					if(Mix_PlayingMusic() <= 0)
					{
						Mix_PlayMusic(music, 0);
					}
				}
				else
				{
					playpause_pressed = 0;
				}
				
				if(SDL_PointInRect(&pt, &rect_button_stop))
				{
					if(Mix_PlayingMusic() != 0)
					{
						stop_pressed = 1;
						Mix_FadeOutMusic(1000);
					}
					else
					{
						stop_pressed = 0;
					}
				}
				else
				{
					stop_pressed = 0;
				}

				if(SDL_PointInRect(&pt, &rect_button_web))
				{
					char url[100];
					snprintf(url, 100, "https://en.wikipedia.org/wiki/%s", Mix_GetMusicArtistTag(music));
					url[99] = '\0';
					SDL_OpenURL(url);
				}

            }
            if(event.type == SDL_MOUSEBUTTONUP || event.type == SDL_KEYUP)
            {
                playpause_pressed = 0;
                stop_pressed = 0;
                //playpause_selected = 0;
                break;
            }
            if(event.type == SDL_MOUSEMOTION)
            {
				const SDL_Point pt = {event.button.x, event.button.y};
				if(SDL_PointInRect(&pt, &rect_button_playpause))
				{
					playpause_selected = 1;
				}
				else
				{
					playpause_selected = 0;
				}
				
				if(SDL_PointInRect(&pt, &rect_button_stop))
				{
					stop_selected = 1;
				}
				else
				{
					stop_selected = 0;
				}
            }
			if(event.type == SDL_DROPFILE)
			{
				loadmusic(event.drop.file);
				loadmetadata();
			}
			if(event.type == SDL_QUIT)
			{
				playing = 1;
				break;
			}
		}

        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 30));
        SDL_BlitSurface(icon_playpause, NULL, surface, &rect_button_playpause);
        SDL_BlitSurface(icon_stop, NULL, surface, &rect_button_stop);
        SDL_BlitSurface(icon_web, NULL, surface, &rect_button_web);

        if(Mix_PlayingMusic())
        {
            SDL_BlitSurface(music_title_playing, NULL, surface, &rect_title);
            SDL_BlitSurface(music_artist_playing, NULL, surface, &rect_artist);
            SDL_BlitSurface(music_album_playing, NULL, surface, &rect_album);
        }
        else
        {
            SDL_BlitSurface(music_title_empty, NULL, surface, &rect_title);
            SDL_BlitSurface(music_artist_empty, NULL, surface, &rect_artist);
            SDL_BlitSurface(music_album_empty, NULL, surface, &rect_album);
        }
    }

    Mix_HaltMusic();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    TTF_CloseFont(font_title);
    TTF_CloseFont(font_others);
    TTF_Quit();
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
