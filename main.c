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
#include "SDL.h"
//#include <SDL2/SDL_image.h>
#include "SDL_mixer.h"
#include "SDL_ttf.h"

const int WIDTH = 600;
const int HEIGHT = 300;

typedef struct music_data
{
    char artist[50];
    char title[100];
    char album[50];
} music_data;

const char **TextSplit(const char *text, char delimiter, int *count)
{
    // thanks raylib

    static const char *result[3] = { NULL };
    static char buffer[150] = { 0 };
    memset(buffer, 0, 150);

    result[0] = buffer;
    int counter = 0;

    if (text != NULL)
    {
        counter = 1;

        for (int i = 0; i < 150; i++)
        {
            buffer[i] = text[i];
            if (buffer[i] == '\0') break;
            else if (buffer[i] == delimiter)
            {
                buffer[i] = '\0';   // Set an end of string at this point
                result[counter] = buffer + i + 1;
                counter++;

                if (counter == 3) break;
            }
        }
    }

    *count = counter;
    return result;
}

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Surface *surface;
    Mix_Music *music;

    window = NULL;
    surface = NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Quit();
        return -1;
    }

    window = SDL_CreateWindow("BASIC MEDIA PLAYER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    /*int imageflags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initimg = IMG_Init(imageflags);
    if((initimg&imageflags) != imageflags)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", IMG_GetError());
        SDL_Quit();
        return -1;
    }*/

    int soundflags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC;
    int initsnd = Mix_Init(soundflags);
    if((initsnd&soundflags) != soundflags)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    if(TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    surface = SDL_GetWindowSurface(window);

    SDL_Surface *music_title_playing;
    SDL_Surface *music_artist_playing;
    SDL_Surface *music_album_playing;
    SDL_Surface *music_title_empty;
    SDL_Surface *music_artist_empty;
    SDL_Surface *music_album_empty;

    SDL_Surface *icon_playpause;
    SDL_Surface *icon_stop;

    SDL_Rect rect_button_playpause;

    SDL_Rect rect_button_stop;

    Uint32 colour_button_unselected = SDL_MapRGB(surface->format, 150, 37, 0);
    Uint32 colour_button_selected = SDL_MapRGB(surface->format, 170, 57, 0);
    Uint32 colour_button_pressed = SDL_MapRGB(surface->format, 0, 50, 255);

    TTF_Font *font_title;
    font_title = TTF_OpenFont("overpass-extralight-italic.otf", 48);
    TTF_Font *font_others;
    font_others = TTF_OpenFont("overpass-extralight-italic.otf", 24);

    SDL_Color color;
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

    music_data music_tags;
    strncpy(music_tags.album, "--", 49);
    strncpy(music_tags.title, "--", 99);
    strncpy(music_tags.artist, "--", 49);

    const Uint16 playpause_text[] = u"⏯";
    const Uint16 stop_text[] = u"⏹";
    icon_playpause = TTF_RenderUNICODE_Blended(font_others, playpause_text, color);
    rect_button_playpause = icon_playpause->clip_rect;
    rect_button_playpause.h = 75;
    rect_button_playpause.w = 75;
    rect_button_playpause.x = (WIDTH / 2) - 110;
    rect_button_playpause.y = HEIGHT - 105;
    icon_stop = TTF_RenderUNICODE_Blended(font_others, stop_text, color);
    rect_button_stop = icon_stop->clip_rect;
    rect_button_stop.h = 45;
    rect_button_stop.w = 45;
    rect_button_stop.x = rect_button_playpause.x + rect_button_playpause.w + 20;
    rect_button_stop.y = HEIGHT - 90;

    SDL_Event event;
    int playing = 0;
    int playpause_selected = 0;
    int playpause_pressed = 0;
    int stop_selected = 0;
    int stop_pressed = 0;

    //check if there are any arguments
    if(argc >= 2)
    {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
        music = Mix_LoadMUS(argv[1]);
        if(music == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "F A I L %s", Mix_GetError());
        }

        Mix_PlayMusic(music, 0);

        SDL_LogDebug(SDL_LOG_CATEGORY_TEST, "%s", Mix_GetMusicTitle(music));

        music_title_playing = TTF_RenderUTF8_Blended(font_title, Mix_GetMusicTitleTag(music), color);
        SDL_Rect rect_title_p = music_title_playing->clip_rect;
        rect_title_p.x = 20;
        rect_title_p.y = 20;
        music_artist_playing = TTF_RenderUTF8_Blended(font_others, Mix_GetMusicArtistTag(music), color);
        SDL_Rect rect_artist_p = music_artist_playing->clip_rect;
        rect_artist_p.x = 20;
        rect_artist_p.y = rect_title_p.y + 70;
        music_album_playing = TTF_RenderUTF8_Blended(font_others, Mix_GetMusicAlbumTag(music), color);
        SDL_Rect rect_album_p = music_album_playing->clip_rect;
        rect_album_p.x = 20;
        rect_album_p.y = rect_artist_p.y + 30;

        char window_title[51];
        snprintf(window_title, 50, "%s - SUPER BASIC MUSIC PLAYER", Mix_GetMusicTitle(music));
        window_title[50] = '\0';

        SDL_SetWindowTitle(window, window_title);
    }

    while(playing == 0)
    {
        SDL_UpdateWindowSurface(window);
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouse_x;
                int mouse_y;

                Uint32 button = SDL_GetMouseState(&mouse_x, &mouse_y);

                if(mouse_x >= rect_button_playpause.x && mouse_x <= rect_button_playpause.x + rect_button_playpause.w)
                {
                    if(mouse_y >= rect_button_playpause.y && mouse_y <= rect_button_playpause.y + rect_button_playpause.h)
                    {
                        if((button & SDL_BUTTON_LMASK) != 0)
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
                    }
                    else
                    {
                        playpause_pressed = 0;
                    }
                }
                else
                {
                    playpause_pressed = 0;
                }

                if(mouse_x >= rect_button_stop.x && mouse_x <= rect_button_stop.x + rect_button_stop.w)
                {
                    if(mouse_y >= rect_button_stop.y && mouse_y <= rect_button_stop.y + rect_button_stop.h)
                    {
                        if((button & SDL_BUTTON_LMASK) != 0 && Mix_PlayingMusic() != 0)
                        {
                            stop_pressed = 1;
                            Mix_FadeOutMusic(1000);
                            //Mix_FreeMusic(music);
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
                }
                else
                {
                    stop_pressed = 0;
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
                int mouse_x;
                int mouse_y;

                Uint32 button = SDL_GetMouseState(&mouse_x, &mouse_y); //button is unused for now

                if(mouse_x >= rect_button_playpause.x && mouse_x <= rect_button_playpause.x + rect_button_playpause.w)
                {
                    if(mouse_y >= rect_button_playpause.y && mouse_y <= rect_button_playpause.y + rect_button_playpause.h)
                    {
                        playpause_selected = 1;
                    }
                    else
                    {
                        playpause_selected = 0;
                    }
                }
                else
                {
                    playpause_selected = 0;
                }

                if(mouse_x >= rect_button_stop.x && mouse_x <= rect_button_stop.x + rect_button_stop.w)
                {
                    if(mouse_y >= rect_button_stop.y && mouse_y <= rect_button_stop.y + rect_button_stop.h)
                    {
                        stop_selected = 1;
                    }
                    else
                    {
                        stop_selected = 0;
                    }
                }
                else
                {
                    stop_selected = 0;
                }
            }

            if(event.type == SDL_QUIT)
            {
                playing = 1;
                break;
            }
        }

        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 30));

        if(playpause_selected == 0 && playpause_pressed == 0)
        {
            SDL_FillRect(surface, &rect_button_playpause, colour_button_unselected);
        }
        else if(playpause_selected == 1 && playpause_pressed == 0)
        {
            SDL_FillRect(surface, &rect_button_playpause, colour_button_selected);
        }
        else if(playpause_selected == 1 && playpause_pressed == 1)
        {
            SDL_FillRect(surface, &rect_button_playpause, colour_button_pressed);
        }
        else
        {
            SDL_FillRect(surface, &rect_button_playpause, colour_button_unselected);
        }

        if(stop_selected == 0 && stop_pressed == 0)
        {
            SDL_FillRect(surface, &rect_button_stop, colour_button_unselected);
        }
        else if(stop_selected == 1 && stop_pressed == 0)
        {
            SDL_FillRect(surface, &rect_button_stop, colour_button_selected);
        }
        else if(stop_selected == 1 && stop_pressed == 1)
        {
            SDL_FillRect(surface, &rect_button_stop, colour_button_pressed);
        }
        else
        {
            SDL_FillRect(surface, &rect_button_playpause, colour_button_unselected);
        }
        SDL_BlitSurface(NULL, &rect_button_playpause, surface, &rect_button_playpause);
        SDL_BlitSurface(surface, &rect_button_stop, surface, &rect_button_stop);

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
        //Mix_getMu
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
