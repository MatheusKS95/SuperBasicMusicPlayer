#Super Basic Music Player
#Copyright (C) 2022  Matheus K. Schaefer
#
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.10)

project(SuperBasicMusicPlayer)

#set(PROJECT_LINK_LIBS1 libSDL2-2.0.so.0.2400.0)
#set(PROJECT_LINK_LIBS2 libSDL2_mixer-2.0.so.0.600.2)
#set(PROJECT_LINK_LIBS3 libSDL2_ttf-2.0.so.0.2000.1)
#link_directories(${PROJECT_SOURCE_DIR}/external/libs)

include_directories(${PROJECT_SOURCE_DIR}/external/include)

add_executable(SuperBasicMusicPlayer main.c)

target_link_libraries(SuperBasicMusicPlayer ${PROJECT_SOURCE_DIR}/external/libs/libSDL2.a)
target_link_libraries(SuperBasicMusicPlayer ${PROJECT_SOURCE_DIR}/external/libs/libSDL2_mixer.a)
target_link_libraries(SuperBasicMusicPlayer ${PROJECT_SOURCE_DIR}/external/libs/libSDL2_ttf.a)
target_link_libraries(SuperBasicMusicPlayer m)
target_link_libraries(SuperBasicMusicPlayer pthread)
target_link_libraries(SuperBasicMusicPlayer freetype)
target_link_libraries(SuperBasicMusicPlayer dl)
