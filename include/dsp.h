/*
 * Apu1 Apple 1 Emulator
 * Copyright (C) 2023 Manjunath C Srinivasa
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#pragma once

#include <common.h>
#include <SDL2/SDL.h>

#define TERMINAL_SPEED 60 // Hz or FPS

typedef struct {
    SDL_Window *window;
    SDL_Surface *screen;
    SDL_Event event;

    // Options
    int scale, height, width;

    // Char map
    Byte charmap[1024], scrtable[960]; // screen table of 40 char per line x 24 lines = 920 bytes
                                       // Apple 1 updated the last 40 bytes of the screen table
                                       // when end of the screen (line 24) was hit. 

    // Positioning
    int indexX, indexY;

    // Timing
    long lasttime;

} dsp_context;

void dsp_init();
void dsp_newline();
void dsp_output();
void dsp_refresh();
void dsp_sync();
void dsp_update();
void dsp_quit();