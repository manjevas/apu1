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

#include <bus.h>

static memory mem;

Byte bus_read(Word address) {
    if(address < 0x8000) {
        return mem.ram[address];
    } else if(address < 0xE000) {
        switch(address) {
            case KBD: 
                mem.pia[1] = 0x27; 
                return mem.pia[0];
            case KBDCR: return mem.pia[1];
            case DSP: return mem.pia[2];
            case DSPCR: return mem.pia[3];
        }
    } else if(address <= 0xFFFF) {
        return mem.rom[address - 0xE000];
    }
}

void bus_write(Word address, Byte value) {
    if(address < 0x8000) {
        mem.ram[address] = value;
        return;
    } else if(address < 0xE000) {
        switch(address) {
            case KBD: mem.pia[0] = value; return;
            case KBDCR: mem.pia[1] = value; return;
            case DSP: mem.pia[2] = value; return;
            case DSPCR: mem.pia[3] = value; return;
        }
    } else if(address <= 0xFFFF) {
        mem.rom[address - 0xE000] = value;
        return;
    }
}

