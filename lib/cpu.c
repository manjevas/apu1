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

#include <cpu.h>
#include <instructions.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx = {0};

Word cpu_read_reg(reg_type rt) {
    switch(rt) {
        case RT_NONE: 
            exit(1);
        case RT_PC:
            return ctx.reg.pc;
        case RT_STATUS:
            return ctx.reg.status;
        case RT_STACK:
            return ctx.reg.stack;
        case RT_ACC:
            return ctx.reg.A;
        case RT_X:
            return ctx.reg.X;
        case RT_Y:
            return ctx.reg.Y;
    }
}

void cpu_write_reg(reg_type rt, Word value) {
    switch(rt) {
        case RT_NONE: 
            return;
        case RT_PC:
            ctx.reg.pc = value & 0xFFFF;
            return;
        case RT_STATUS:
            ctx.reg.status = value & 0xFF;
            return;
        case RT_STACK:
            ctx.reg.stack = value & 0xFF;
            return;
        case RT_ACC:
            ctx.reg.A = value & 0xFF;
            return;
        case RT_X:
            ctx.reg.X = value & 0xFF;
            return;
        case RT_Y:
            ctx.reg.Y = value & 0xFF;
            return;
    }
}

bool cpu_read_flag(flag_type ft) {
    Byte tmp = ctx.reg.status;
    return (tmp & (1 << ft)) ? 1 : 0;
}

void cpu_write_flag(flag_type ft, bool value) {
    if(value) {
        ctx.reg.status |= 1 << ft;
    } else {
        ctx.reg.status &= ~(1 << ft);
    }    
}

void cpu_init() {
    // Reset registers
    cpu_write_reg(RT_STACK, 0xFF);
    cpu_write_reg(RT_ACC, 0);
    cpu_write_reg(RT_X, 0);
    cpu_write_reg(RT_Y, 0);
    cpu_write_flag(FT_U, 1);

    // Goto PC indicated by reset vector
    cpu_write_reg(RT_PC, 
                  bus_read(0xFFFC) | 
                  bus_read(0xFFFD) << 8);

    // Enable CPU halt, stepping on and enable debug mode
    cpu_halt(true);
    cpu_stepping(true);
    cpu_debug(true);
}

void cpu_fetch_instruction() {
    ctx.op_code = bus_read(ctx.reg.pc++);
    ctx.inst = instruction_by_opcode(ctx.op_code);
}

void cpu_fetch_data() {
    switch(ctx.inst->mode) {
        case AM_IMP: 
            return;
        case AM_ACC:
            ctx.fetched_data = cpu_read_reg(RT_ACC);
            return;
        case AM_ZPG: {
            Byte zpgAddr = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Word memAddr = 0x0000 + zpgAddr;                    
            emu_cycles(1);
            ctx.mem_addr = memAddr;
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc++;
            return; 
            }
        case AM_ZPX: {
            Byte zpgAddr;
            Byte xInd;
            Word memAddr;
            zpgAddr = bus_read(ctx.reg.pc);
            emu_cycles(1);
            xInd = cpu_read_reg(RT_X);
            emu_cycles(1);
            memAddr = 0x0000 + (zpgAddr + xInd) & 0xFF ; // Increment without carry
            ctx.mem_addr = memAddr;
            emu_cycles(1);
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc++;
            return; 
            }
        case AM_ZPY: {
            Byte zpgAddr;
            Byte yInd;
            Word memAddr;
            zpgAddr = bus_read(ctx.reg.pc);
            emu_cycles(1);
            yInd = cpu_read_reg(RT_Y);
            emu_cycles(1);
            memAddr = 0x0000 + (zpgAddr + yInd) & 0xFF ; // Increment without carry
            ctx.mem_addr = memAddr;
            emu_cycles(1);
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc++;
            return; 
            }
        case AM_ABS: {
            Byte lo = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Byte hi = bus_read(ctx.reg.pc + 1);
            emu_cycles(1);
            Word memAddr = lo | (hi << 8);
            emu_cycles(1);
            ctx.mem_addr = memAddr;
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc += 2;
            return;
        }
        case AM_ABX: {
            Byte lo = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Byte hi = bus_read(ctx.reg.pc + 1);
            emu_cycles(1);
            Byte xInd = cpu_read_reg(RT_X);
            emu_cycles(1);
            Word memAddr = (lo | (hi << 8)) + xInd;     
            // Add a cycle if mem address crosses a page
            if((memAddr & 0xFF00) != ((lo | (hi << 8)) & 0xFF00)) emu_cycles(1);
            ctx.mem_addr = memAddr;
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc += 2;
            return;
        }
        case AM_ABY: {
            Byte lo = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Byte hi = bus_read(ctx.reg.pc + 1);
            emu_cycles(1);
            Byte yInd = cpu_read_reg(RT_Y);
            emu_cycles(1);
            Word memAddr = (lo | (hi << 8)) + yInd;      
            // Add a cycle if mem address crosses a page
            if((memAddr & 0xFF00) != ((lo | (hi << 8)) & 0xFF00)) emu_cycles(1);
            ctx.mem_addr = memAddr;
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc += 2;
            return;
        }
        case AM_IND: {
            Byte lo = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Byte hi = bus_read(ctx.reg.pc + 1);
            emu_cycles(1);
            Word memAddr = (lo | (hi << 8)); 
            Byte l = bus_read(memAddr);
            Byte h = bus_read(memAddr + 1);
            Word MemAddr = (l | (h << 8));      
            ctx.mem_addr = memAddr;
            emu_cycles(1);
            ctx.fetched_data = bus_read(memAddr);
            emu_cycles(1);
            ctx.reg.pc += 2;
            return;
        }
        case AM_XID: {
            Byte zpgAddr = bus_read(ctx.reg.pc);
            emu_cycles(1);            
            Byte xInd = cpu_read_reg(RT_X);
            emu_cycles(1);
            Word memAddr = 0x0000 + zpgAddr + xInd;                    
            emu_cycles(1);
            Byte lo = bus_read(memAddr);
            emu_cycles(1);
            memAddr += 1;
            Byte hi = bus_read(memAddr);
            ctx.mem_addr = lo | (hi << 8);
            emu_cycles(1);
            ctx.fetched_data = bus_read(lo | (hi << 8));
            emu_cycles(1);
            ctx.reg.pc++;
            return;
        }
        case AM_IDY: {
            Byte zpgAddr = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Word memAddr = 0x0000 + zpgAddr;                    
            emu_cycles(1);
            Byte lo = bus_read(memAddr);
            emu_cycles(1);
            Byte yInd = cpu_read_reg(RT_Y);
            emu_cycles(1);
            memAddr += 1;
            Byte hi = bus_read(memAddr);
            ctx.mem_addr = (lo | (hi << 8)) + yInd;
            emu_cycles(1);
            ctx.fetched_data = bus_read((lo | (hi << 8)) + yInd);
            // Add a cycle if mem address crosses a page
            if((ctx.reg.pc & 0xFF00) != (((lo | (hi << 8)) + yInd) & 0xFF00)) emu_cycles(1);
            ctx.reg.pc++;
            return;
        }
        case AM_IMM: {
            ctx.fetched_data = bus_read(ctx.reg.pc) & 0xFF;
            emu_cycles(1);
            ctx.reg.pc++;
            return;
        }
        case AM_REL:{
            ctx.fetched_data = bus_read(ctx.reg.pc);
            emu_cycles(1);
            Word memAddr;
            if(!(ctx.fetched_data >> 7)) // not negative 
                memAddr = ctx.reg.pc + (ctx.fetched_data & 0x7F);
            else // negetive, 2's complement
                memAddr = ctx.reg.pc - 256 + (ctx.fetched_data & 0xFF);
            ctx.mem_addr = memAddr + 1;

            // Add a cycle if mem address crosses a page
            if((memAddr & 0xFF00) != (ctx.reg.pc & 0xFF00)) emu_cycles(1);
            
            emu_cycles(1);
            ctx.reg.pc++;
            return;
        }
        default:
            printf("Unkown Addressing Mode! %d\n", ctx.inst->mode); 
            exit(-6);   
            return;
    }
}

void cpu_execute() {
    INST_PROC proc = inst_get_process(ctx.inst->type);

    if (!proc) NO_IMPL

    proc(&ctx);
}

bool cpu_step() {
    if(!ctx.halted) {
        cpu_fetch_instruction();
        emu_cycles(1);
        cpu_fetch_data();        
        cpu_execute();
        
        if(ctx.debug) {
            // Debug print
            emu_context *emu_ctx = emu_get_context();
            printf("Cycle: %06X\n", emu_ctx->ticks);
            printf("Registers:\n");
            printf("PC\tSTACK\tACC\tX\tY\n");        
            printf("%04X\t%02X\t%02X\t%02X\t%02X\n\n", ctx.reg.pc, 
            ctx.reg.stack, ctx.reg.A, ctx.reg.X, 
            ctx.reg.Y);
            printf("N, V, U, B, D, I, Z, C\n");
            printf("%01i, %01i, %01i, %01i, %01i, %01i, %01i, %01i\n\n", 
            cpu_read_flag(FT_N), cpu_read_flag(FT_V), cpu_read_flag(FT_U), 
            cpu_read_flag(FT_B), cpu_read_flag(FT_D), cpu_read_flag(FT_I), 
            cpu_read_flag(FT_Z), cpu_read_flag(FT_C));
            printf("CPU Fetch: \n");
            printf("Cur Opcode\t= %02X\nCur Inst\t= %s\nFetched Data\t= %02X\nMem Addr\t= %04X\n\n", 
                    ctx.op_code, inst_name(ctx.inst->type), ctx.fetched_data,
                    ctx.mem_addr);
        }

        if(ctx.stepping) return false;
        return true;
    }
    return false;
}

void *cpu_run(void *p) {
    cpu_init();

    while(emu_get_context()->running) {
        if(emu_get_context()->paused) {
            emu_delay(10);
            continue;
        }

        if(!cpu_step()) {
            printf("CPU Stopped\n");
            while(cpu_get_context()->stepping) {
                emu_delay(100);
            } 
        }
    }
}

cpu_context *cpu_get_context() {
    return &ctx;
}

void cpu_stepping(bool on) {
    ctx.stepping = on; 
}

void cpu_halt(bool on) {
    ctx.halted = on;
}

void cpu_debug(bool on) {
    ctx.debug = on;
}