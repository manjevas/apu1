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

typedef struct {
    bool running;
    bool paused;
    bool die;

    u64 ticks;
} emu_context;

int emu_run(int arc, char **argv);
void emu_cycles(int cycles);
bool emu_load_monitor(void);
bool emu_init(void);
void emu_delay(int ms);
emu_context *emu_get_context();