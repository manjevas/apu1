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

#include <dsp.h>
#include <bus.h>

static dsp_context ctx;

bool dsp_load_charmap() {
    char fileName[20] = "../roms/charmap.rom";
    FILE* fd = fopen(fileName, "r");

    // checking if the file exist or not
    if (fd == NULL) {
        printf("charmap.rom file not found!\n");
        return -1;
    }

    fseek(fd, 0L, SEEK_END);
  
    // calculating the size of the file
    long int fsize = ftell(fd);

    // close the file
    fclose(fd);

    size_t s = 0;
    Byte buf[fsize];
    int ii, jj;

    fd = fopen(fileName, "r");
    if(fd) {
        s = fread(&ctx.charmap, 1, fsize + 1, fd);
        fclose(fd);
    }

    if(s) {
        printf("Charmap Loaded.\n");
    } else {
        printf("Failed to open charmap file.\n");
        return false;
    }

    return true;
}

void dsp_init() {
    ctx.scale = 3;
    ctx.height = 24 * ctx.scale  * 8;
    ctx.width = 40 * ctx.scale  * 8;

    //Initialize screen table
    ctx.indexX = 0;
    ctx.indexY = 0;
    memset(ctx.scrtable, 0, 960);

    //Initialize timing
    ctx.lasttime = SDL_GetTicks();

    // Load Character set
    dsp_load_charmap();

    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    //Create window
    ctx.window = SDL_CreateWindow("Apple emUlator 1", SDL_WINDOWPOS_UNDEFINED, 
                                    SDL_WINDOWPOS_UNDEFINED, ctx.width, ctx.height, 
                                    SDL_WINDOW_SHOWN);
    if(ctx.window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    //Get window surface
    ctx.screen = SDL_GetWindowSurface(ctx.window);

    //Fill the surface white
    SDL_FillRect(ctx.screen, NULL, SDL_MapRGB(ctx.screen->format, 0x00, 0x00, 0x00));
    
    //Update the surface
    // dsp_refresh();
    SDL_UpdateWindowSurface(ctx.window);
}

void dsp_quit() {
    //Destroy window
    SDL_DestroyWindow(ctx.window);

    //Quit SDL subsystems
    SDL_Quit();
}

void dsp_newline() {
    memcpy(&ctx.scrtable, &ctx.scrtable[40], 920); // copy in 40th to 920th byte to 0th location, effectively erasing 
                                                   // first line from memory
	memset(&ctx.scrtable[920], 0, 40);             // Fill last 40 bytes with 0, waiting for user input
}

void dsp_output() {
    Byte dsp = bus_read(DSP);
    dsp &= 0x7F; // clear bit 7 of dsp to let monitor continue to next char
    
    if(dsp == 0x0D) { // CR
        ctx.indexX = 0;
        ctx.indexY++; // new line
    } else if(dsp > 0x1F && dsp < 0x60) { // symbols, digits and uppercase char
        ctx.scrtable[ctx.indexY * 40 + ctx.indexX] = dsp;
        ctx.indexX++;
    }

    if(ctx.indexX == 40) { // Hit end of line, new line
        ctx.indexX = 0;
        ctx.indexY++;
    }

    if(ctx.indexY == 24) { // Hit end of screen, push out old chars from line 0 and plug in new chars in line 23
        dsp_newline();
        ctx.indexY--;
    }
    
    bus_write(DSP, dsp);
}

void dsp_draw_character(int xPosition, int yPosition, Byte r, Byte g, Byte b, Byte characNumber) {
	SDL_Rect rect;
	int k, l;

	for (k = 0; k < 8; k++)
	{
		for (l = 1; l < 8; l++)
		{
			if (ctx.charmap[characNumber * 8 + k] & (0x01 << l)) // each byte in charmap has bit 1 thru 7 describing 
                                                                 // pixel status at each line in the dot matrix 
                                                                 // (8 rows by 7 cols)
			{
				rect.x = xPosition + ctx.scale * (l - 1);
				rect.y = yPosition + ctx.scale * k;
				rect.w = ctx.scale;
				rect.h = ctx.scale;

				SDL_FillRect(ctx.screen, &rect, SDL_MapRGB(ctx.screen->format, r, g, b));
			}
		}
	}
}

void dsp_refresh() {
    int xPosition, yPosition;
	int i, j;

	SDL_FillRect(ctx.screen, NULL, 0);
		
	for (i = 0; i < 40; i++)
	{
		for (j = 0; j < 24; j++)
		{
			xPosition = i * ctx.scale * 7;
			yPosition = j * ctx.scale * 8;
				
			dsp_draw_character(xPosition, yPosition, 0, 255, 0, ctx.scrtable[j * 40 + i]); // will draw green characters
		}
	}

	dsp_draw_character(ctx.indexX * ctx.scale * 7, ctx.indexY * ctx.scale * 8, 0, 255, 0, 0x40); // @ cursor

    SDL_UpdateWindowSurface(ctx.window);
}

void dsp_sync() {
    int sleep_ms = ((1000 / TERMINAL_SPEED) - (SDL_GetTicks() - ctx.lasttime));

    if (sleep_ms > 0)
		SDL_Delay(sleep_ms);

	ctx.lasttime = SDL_GetTicks();
}


void dsp_update() {
    if(bus_read(DSP) & 0x80) { // Bit 7 of DSP set? Output to screen
        dsp_output();
        dsp_refresh();
        dsp_sync();
    } 
}