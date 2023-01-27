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

#define KBD 0xD010
#define KBDCR 0xD011
#define DSP 0xD012
#define DSPCR 0xD013

typedef struct {
    Byte rom[0x2000];
    Byte ram[0x8000];
    Byte pia[0x0004];
} memory;

Byte bus_read(Word address);
void bus_write(Word address, Byte value);