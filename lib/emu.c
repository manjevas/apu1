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

#include <emu.h>
#include <cpu.h>
#include <instructions.h>
#include <bus.h>
#include <dsp.h>
#include <kbd.h>
#include <SDL2/SDL.h>

#include <pthread.h>
#include <unistd.h>

static emu_context ctx;

bool emu_load_monitor(void) {
    char fileName[20] = "../roms/monitor.rom";
    Word startAddr = 0xFF00;
    FILE* fd = fopen(fileName, "r");

    // checking if the file exist or not
    if (fd == NULL) {
        printf("File Not Found!\n");
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
        s = fread(&buf, 1, fsize + 1, fd);
        fclose(fd);
    }

    if(s) {
        printf("Woz Monitor Loaded.\n");
    } else {
        printf("Failed to open Woz Monitor file.\n");
        return false;
    }

    jj = 0;
    for(ii = startAddr; ii < startAddr + fsize - 2; ii++) {
        bus_write(ii, buf[jj++]);
    }
    return true;
}

bool emu_init(void) {
    if(emu_load_monitor()) {
        cpu_init();
        ctx.ticks = 0;
        ctx.running = true;
        ctx.paused = false;
        ctx.die = false;
        return true;
    }
    return false;
}

void emu_cycles(int cycles) {
    ctx.ticks += cycles;
    emu_delay(1000/CPU_SPEED);
}

void emu_delay(int ms) {
    SDL_Delay(ms);
}

int emu_run(int argc, char **argv) {
    dsp_init();

    pthread_t t1;

    if (pthread_create(&t1, NULL, cpu_run, NULL)) {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
    return -1;
    }
    
    while(!ctx.die) {
        kbd_handle();
        dsp_update();
    }
    dsp_quit();
    return 0;
}

emu_context *emu_get_context() {
    return &ctx;
}