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

#define CPU_SPEED 1000000 // Hz

typedef enum {
    FT_C,
    FT_Z,
    FT_I,
    FT_D,
    FT_B,
    FT_U,
    FT_V,
    FT_N
} flag_type;

typedef enum {
    RT_NONE, 
    RT_PC, 
    RT_STATUS, 
    RT_STACK, 
    RT_ACC, 
    RT_X, 
    RT_Y
} reg_type;

typedef struct {
    Word pc;
    Byte stack, status, A, X, Y;
} regs;

typedef enum {
    IN_ILL,
    // Transfer
    IN_LD,      // LDA, LDX, LDY 
    IN_ST,      // STA, STX, STY
    IN_REG,     // TAX, TXA, DEX, INX, TAY, TYA, DAY, INY, TXS,TSX,
    // Stack
    IN_STC,     // PHA,PLA,PHP,PLP
    // Decrement & Increment
    IN_DEC,     // DEC, DEX, DEY
    IN_INC,     // INC, INX, INY
    // Arithmetic
    IN_ADC,
    IN_SBC,
    // Logical
    IN_AND,
    IN_EOR,
    IN_ORA,
    // Shift & Rotate
    IN_ASL,
    IN_LSR,
    IN_ROL,
    IN_ROR,
    // Flag
    IN_FLG,     // CLC, CLD, CLI, CLV, SEC, SED, SEI
    // Comparison
    IN_CMP,     // CPX, CPY, CMP
    // Branch 
    IN_BCH,     // BCC, BCS, BEQ, BMI, BNE, BPL, BVX, BVS
    // Jump & Subroutine
    IN_JMP, 
    IN_JSR,
    IN_RTS,
    // Interrupts
    IN_BRK,
    IN_RTI,
    // Other
    IN_BIT,
    IN_NOP
} inst_type;

typedef enum {
    AM_NONE,
    AM_ACC,
    AM_ABS,
    AM_ABX,
    AM_ABY,
    AM_IMM,
    AM_IMP,
    AM_IND,
    AM_XID,
    AM_IDY,
    AM_REL,
    AM_ZPG,
    AM_ZPX,
    AM_ZPY
} addr_mode;

typedef enum {
    CT_NONE, CT_N, CT_NN, CT_NZ, CT_Z, CT_NC, CT_C, CT_V, CT_NV,
} cond_type;

typedef struct {
    inst_type type;
    addr_mode mode;
    reg_type reg1;
    reg_type reg2;
    cond_type cond;
} instruction;

instruction *instruction_by_opcode(Byte opcode);

typedef struct {
    regs reg;

    // Fetched data
    Byte op_code;
    Byte fetched_data; // data
    Word mem_addr; // mem addr
    instruction *inst;

    bool stepping;
    bool halted;
    bool debug;
} cpu_context;

typedef void (*INST_PROC)(cpu_context *);

// Utils
Word cpu_read_reg(reg_type rt);
void cpu_write_reg(reg_type rt, Word value);
bool cpu_read_flag(flag_type ft);
void cpu_write_flag(flag_type ft, bool value);

// Init
void cpu_init();

// Run
bool cpu_step();
void cpu_fetch_data();
void cpu_fetch_instruction();
void cpu_execute();
void *cpu_run(void *p);

// Controls
void cpu_stepping(bool on);
void cpu_halt(bool on);
void cpu_debug(bool on);

// Export
cpu_context *cpu_get_context();