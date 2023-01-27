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
#include <cpu.h>

// Utils
void set_status(cpu_context *ctx, Byte val);
bool check_cond(cpu_context *ctx);

// Processes
void proc_none(cpu_context *ctx);
void proc_ld(cpu_context *ctx);
void proc_st(cpu_context *ctx);
void proc_reg(cpu_context *ctx);
void proc_stc(cpu_context *ctx);
void proc_dec(cpu_context *ctx);
void proc_inc(cpu_context *ctx);
void proc_adc(cpu_context *ctx);
void proc_sbc(cpu_context *ctx);
void proc_and(cpu_context *ctx);
void proc_eor(cpu_context *ctx);
void proc_ora(cpu_context *ctx);
void proc_asl(cpu_context *ctx);
void proc_lsr(cpu_context *ctx);
void proc_rol(cpu_context *ctx);
void proc_ror(cpu_context *ctx);
void proc_flg(cpu_context *ctx);
void proc_cmp(cpu_context *ctx);
void proc_bch(cpu_context *ctx);
void proc_jmp(cpu_context *ctx);
void proc_jsr(cpu_context *ctx);
void proc_rts(cpu_context *ctx);
void proc_brk(cpu_context *ctx);
void proc_rti(cpu_context *ctx);
void proc_bit(cpu_context *ctx);
void proc_nop(cpu_context *ctx);
INST_PROC inst_get_process(inst_type type);

char *inst_name(inst_type t);