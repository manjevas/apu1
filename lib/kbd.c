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

#include <kbd.h>
#include <emu.h>
#include <cpu.h>
#include <bus.h>
#include <dsp.h>
#include <pthread.h>

static kbd_context ctx;

void kbd_handle() {    
    SDL_StartTextInput();
    SDL_PollEvent(&ctx.event);
    // Handle quit
    if(ctx.event.type == SDL_WINDOWEVENT && ctx.event.window.event == SDL_WINDOWEVENT_CLOSE) {
        printf("\nQuitting...\n");
        emu_delay(1000); // 1 s wait 
        emu_get_context()->die = true;
    }

    // Handle all other key events
    if(ctx.event.type == SDL_KEYDOWN) {
        ctx.key = ctx.event.key.keysym.sym;

        switch(ctx.key) {
            case SDLK_F1: { // HELP!
                printf("\nKey Bindings:\n");
                printf("F1 - Help with key bindings.\n");
                printf("F2 - Toggle CPU debug mode.\n");
                printf("F3 - Step CPU one instruction at a time.\n");
                printf("F4 - Run CPU.\n");
                printf("F5 - Reset CPU.\n");                
                printf("F12 - Quit.\n");
                printf("\n");
                break;
            }
            case SDLK_F2: { // Toggle debug 
                printf(cpu_get_context()->debug ? "\nCPU Debugging OFF\n" : "\nCPU Debugging ON\n");
                cpu_get_context()->debug ? cpu_debug(false) : cpu_debug(true);
                break;
            }
            case SDLK_F3: { // Step CPU  
                printf("\nStepping CPU\n");
                cpu_halt(false); 
                cpu_stepping(true);
                cpu_step();
                break;
            }
            case SDLK_F4: { // Run CPU  
                cpu_halt(false);                
                cpu_stepping(false);

                break;
            }
            case SDLK_F5: { //  Reset CPU 
                printf("\nResetting CPU\n");
                cpu_halt(false);                
                cpu_stepping(false);
                cpu_init();
                dsp_init();
                break;
            }            
            case SDLK_F12: { //  Quitting EMU
                printf("\nQuitting...\n");
                emu_delay(1000); // 1 s wait 
                emu_get_context()->die = true;
                break;
            }
        }
        
        // Handle ASCII
        if(ctx.key > 95 && ctx.key < 128) { // lower case letters
            printf("KEY ENTERED: %c [0x%02X]\n", ctx.key - 32, ctx.key - 32);
            bus_write(KBD, (ctx.key - 32) | 0x80);
            bus_write(KBDCR, 0xA7);
        } else if(ctx.key > 31 && ctx.key < 96) { // symbols, digits and upper case letters
            printf("KEY ENTERED: %c [0x%02X]\n", ctx.key, ctx.key);
            bus_write(KBD, ctx.key| 0x80);
            bus_write(KBDCR, 0xA7);
        } else if(ctx.key == 13) { // CR   
            printf("KEY ENTERED: %c [0x%02X]\n", ctx.key, ctx.key);     
            bus_write(KBD, ctx.key| 0x80);
            bus_write(KBDCR, 0xA7);
        }
        
        emu_delay(250); // delay to avoid multiple key press
    }
    
    ctx.key = 0; // reset key press
}