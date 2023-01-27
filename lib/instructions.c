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
#include <instructions.h>
#include <cpu.h>
#include <bus.h>

instruction instructions[0x100] = {
    // 0x0...   
     [0x00] = {IN_BRK,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x01] = {IN_ORA,  AM_XID,  RT_NONE,  RT_NONE, CT_NONE},
     [0x02] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x03] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x04] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x05] = {IN_ORA,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x06] = {IN_ASL,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x07] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x08] = {IN_STC,  AM_IMP,  RT_STATUS,RT_STACK,CT_NONE},
     [0x09] = {IN_ORA,  AM_IMM,  RT_NONE,  RT_NONE, CT_NONE},
     [0x0A] = {IN_ASL,  AM_ACC,  RT_NONE,  RT_NONE, CT_NONE},
     [0x0B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x0C] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x0D] = {IN_ORA,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x0E] = {IN_ASL,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x0F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0x1...   
     [0x10] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_NN},
     [0x11] = {IN_ORA,  AM_IDY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x12] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x13] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x14] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x15] = {IN_ORA,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x16] = {IN_ASL,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x17] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x18] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //CLC
     [0x19] = {IN_ORA,  AM_ABY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x1A] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x1B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x1C] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x1D] = {IN_ORA,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x1E] = {IN_ASL,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x1F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, 
     // 0x2...   
     [0x20] = {IN_JSR,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x21] = {IN_AND,  AM_XID,  RT_NONE,  RT_NONE, CT_NONE},
     [0x22] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x23] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x24] = {IN_BIT,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x25] = {IN_AND,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x26] = {IN_ROL,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x27] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x28] = {IN_STC,  AM_IMP,  RT_STACK, RT_STATUS,CT_NONE},
     [0x29] = {IN_AND,  AM_IMM,  RT_NONE,  RT_NONE, CT_NONE},
     [0x2A] = {IN_ROL,  AM_ACC,  RT_NONE,  RT_NONE, CT_NONE},
     [0x2B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x2C] = {IN_BIT,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x2D] = {IN_AND,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x2E] = {IN_ROL,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x2F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0x3...   
     [0x30] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_N},
     [0x31] = {IN_AND,  AM_IDY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x32] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x33] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x34] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x35] = {IN_AND,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x36] = {IN_ROL,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x37] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x38] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //SEC
     [0x39] = {IN_AND,  AM_ABY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x3A] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x3B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x3C] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x3D] = {IN_AND,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x3E] = {IN_ROL,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x3F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0x4...   
     [0x40] = {IN_RTI,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x41] = {IN_EOR,  AM_XID,  RT_NONE,  RT_NONE, CT_NONE},
     [0x42] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x43] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x44] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x45] = {IN_EOR,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x46] = {IN_LSR,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x47] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x48] = {IN_STC,  AM_IMP,  RT_ACC,   RT_STACK,CT_NONE},
     [0x49] = {IN_EOR,  AM_IMM,  RT_NONE,  RT_NONE, CT_NONE},
     [0x4A] = {IN_LSR,  AM_ACC,  RT_NONE,  RT_NONE, CT_NONE},
     [0x4B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x4C] = {IN_JMP,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x4D] = {IN_EOR,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x4E] = {IN_LSR,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x4F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0x5...   
     [0x50] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_NV},
     [0x51] = {IN_EOR,  AM_IDY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x52] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x53] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x54] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x55] = {IN_EOR,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x56] = {IN_LSR,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x57] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x58] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //CLI
     [0x59] = {IN_EOR,  AM_ABY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x5A] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x5B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x5C] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x5D] = {IN_EOR,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x5E] = {IN_LSR,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x5F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},  

     // 0x6...   
     [0x60] = {IN_RTS,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x61] = {IN_ADC,  AM_XID,  RT_NONE,  RT_NONE, CT_NONE},
     [0x62] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x63] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x64] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x65] = {IN_ADC,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x66] = {IN_ROR,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0x67] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x68] = {IN_STC,  AM_IMP,  RT_STACK, RT_ACC,  CT_NONE},
     [0x69] = {IN_ADC,  AM_IMM,  RT_NONE,  RT_NONE, CT_NONE},
     [0x6A] = {IN_ROR,  AM_ACC,  RT_NONE,  RT_NONE, CT_NONE},
     [0x6B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x6C] = {IN_JMP,  AM_IND,  RT_NONE,  RT_NONE, CT_NONE},
     [0x6D] = {IN_ADC,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x6E] = {IN_ROR,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0x6F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0x7...   
     [0x70] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_V},
     [0x71] = {IN_ADC,  AM_IDY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x72] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x73] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x74] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x75] = {IN_ADC,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x76] = {IN_ROR,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x77] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x78] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //SEI
     [0x79] = {IN_ADC,  AM_ABY,  RT_NONE,  RT_NONE, CT_NONE},
     [0x7A] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x7B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x7C] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x7D] = {IN_ADC,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x7E] = {IN_ROR,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0x7F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0x8.. 
     [0x80] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x81] = {IN_ST,   AM_XID,  RT_ACC,   RT_NONE, CT_NONE},
     [0x82] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x83] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x84] = {IN_ST,   AM_ZPG,  RT_Y,     RT_NONE, CT_NONE},
     [0x85] = {IN_ST,   AM_ZPG,  RT_ACC,   RT_NONE, CT_NONE},
     [0x86] = {IN_ST,   AM_ZPG,  RT_X,     RT_NONE, CT_NONE},
     [0x87] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x88] = {IN_DEC,  AM_IMP,  RT_Y,     RT_NONE, CT_NONE},
     [0x89] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x8A] = {IN_REG,  AM_IMP,  RT_ACC,   RT_X,    CT_NONE},
     [0x8B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x8C] = {IN_ST,   AM_ABS,  RT_Y,     RT_NONE, CT_NONE},
     [0x8D] = {IN_ST,   AM_ABS,  RT_ACC,   RT_NONE, CT_NONE},
     [0x8E] = {IN_ST,   AM_ABS,  RT_X,     RT_NONE, CT_NONE},
     [0x8F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
        
     // 0x9...   
     [0x90] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_NC},
     [0x91] = {IN_ST,   AM_IDY,  RT_ACC,   RT_NONE, CT_NONE},
     [0x93] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x92] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x94] = {IN_ST,   AM_ZPX,  RT_Y,     RT_NONE, CT_NONE},
     [0x95] = {IN_ST,   AM_ZPX,  RT_ACC,   RT_NONE, CT_NONE},
     [0x96] = {IN_ST,   AM_ZPY,  RT_X,     RT_NONE, CT_NONE},
     [0x97] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x98] = {IN_REG,  AM_IMP,  RT_ACC,   RT_Y,    CT_NONE},
     [0x99] = {IN_ST,   AM_ABY,  RT_ACC,   RT_NONE, CT_NONE},
     [0x9A] = {IN_REG,  AM_IMM,  RT_STACK, RT_X,    CT_NONE},
     [0x9B] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x9C] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x9D] = {IN_ST,   AM_ABX,  RT_ACC,   RT_NONE, CT_NONE},
     [0x9E] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0x9F] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0xA...   
     [0xA0] = {IN_LD,   AM_IMM,  RT_Y,     RT_NONE, CT_NONE},
     [0xA1] = {IN_LD,   AM_XID,  RT_ACC,   RT_NONE, CT_NONE},
     [0xA2] = {IN_LD,   AM_IMM,  RT_X,     RT_NONE, CT_NONE},
     [0xA3] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xA4] = {IN_LD,   AM_ZPG,  RT_Y,     RT_NONE, CT_NONE},
     [0xA5] = {IN_LD,   AM_ZPG,  RT_ACC,   RT_NONE, CT_NONE},
     [0xA6] = {IN_LD,   AM_ZPG,  RT_X,     RT_NONE, CT_NONE},
     [0xA7] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xA8] = {IN_REG,  AM_IMP,  RT_Y,     RT_ACC,  CT_NONE},
     [0xA9] = {IN_LD,   AM_IMM,  RT_ACC,   RT_NONE, CT_NONE},
     [0xAA] = {IN_REG,  AM_IMP,  RT_X,     RT_ACC,  CT_NONE},
     [0xAB] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xAC] = {IN_LD,   AM_ABS,  RT_Y,     RT_NONE, CT_NONE},
     [0xAD] = {IN_LD,   AM_ABS,  RT_ACC,   RT_NONE, CT_NONE},
     [0xAE] = {IN_LD,   AM_ABS,  RT_X,     RT_NONE, CT_NONE},
     [0xAF] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0xB...    
     [0xB0] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_C},
     [0xB1] = {IN_LD,   AM_IDY,  RT_ACC,   RT_NONE, CT_NONE},
     [0xB2] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xB3] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xB4] = {IN_LD,   AM_ZPX,  RT_Y,     RT_NONE, CT_NONE},
     [0xB5] = {IN_LD,   AM_ZPX,  RT_ACC,   RT_NONE, CT_NONE},
     [0xB6] = {IN_LD,   AM_ZPY,  RT_X,     RT_NONE, CT_NONE},
     [0xB7] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xB8] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //CLV
     [0xB9] = {IN_LD,   AM_ABY,  RT_ACC,   RT_NONE, CT_NONE},
     [0xBA] = {IN_REG,  AM_IMP,  RT_X,     RT_STACK,CT_NONE},
     [0xBB] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xBC] = {IN_LD,   AM_ABX,  RT_Y,     RT_NONE, CT_NONE},
     [0xBD] = {IN_LD,   AM_ABX,  RT_ACC,   RT_NONE, CT_NONE},
     [0xBE] = {IN_LD,   AM_ABY,  RT_X,     RT_NONE, CT_NONE},
     [0xBF] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0xC...   
     [0xC0] = {IN_CMP,  AM_IMM,  RT_Y,     RT_NONE, CT_NONE},
     [0xC1] = {IN_CMP,  AM_XID,  RT_ACC,   RT_NONE, CT_NONE},
     [0xC2] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xC3] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xC4] = {IN_CMP,  AM_ZPG,  RT_Y,     RT_NONE, CT_NONE},
     [0xC5] = {IN_CMP,  AM_ZPG,  RT_ACC,   RT_NONE, CT_NONE},
     [0xC6] = {IN_DEC,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0xC7] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xC8] = {IN_INC,  AM_IMP,  RT_Y,     RT_NONE, CT_NONE},
     [0xC9] = {IN_CMP,  AM_IMM,  RT_ACC,   RT_NONE, CT_NONE},
     [0xCA] = {IN_DEC,  AM_IMP,  RT_X,     RT_NONE, CT_NONE},
     [0xCB] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xCC] = {IN_CMP,  AM_ABS,  RT_Y,     RT_NONE, CT_NONE},
     [0xCD] = {IN_CMP,  AM_ABS,  RT_ACC,   RT_NONE, CT_NONE},
     [0xCE] = {IN_DEC,  AM_IMP,  RT_ACC,   RT_NONE, CT_NONE},
     [0xCF] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0xD...   
     [0xD0] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_NZ},
     [0xD1] = {IN_CMP,  AM_IDY,  RT_ACC,   RT_NONE, CT_NONE},
     [0xD2] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xD3] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xD4] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xD5] = {IN_CMP,  AM_ZPX,  RT_ACC,   RT_NONE, CT_NONE},
     [0xD6] = {IN_DEC,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0xD7] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xD8] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //CLD
     [0xD9] = {IN_CMP,  AM_ABY,  RT_ACC,   RT_NONE, CT_NONE},
     [0xDA] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xDB] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xDC] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xDD] = {IN_CMP,  AM_ABX,  RT_ACC,   RT_NONE, CT_NONE},
     [0xDE] = {IN_DEC,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0xDF] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, 

     // 0xE...   
     [0xE0] = {IN_CMP,  AM_IMM,  RT_X,     RT_NONE, CT_NONE},
     [0xE1] = {IN_SBC,  AM_XID,  RT_NONE,  RT_NONE, CT_NONE},
     [0xE2] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xE3] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xE4] = {IN_CMP,  AM_ZPG,  RT_X,     RT_NONE, CT_NONE},
     [0xE5] = {IN_SBC,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0xE6] = {IN_INC,  AM_ZPG,  RT_NONE,  RT_NONE, CT_NONE},
     [0xE7] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xE8] = {IN_INC,  AM_IMP,  RT_X,     RT_NONE, CT_NONE},
     [0xE9] = {IN_SBC,  AM_IMM,  RT_NONE,  RT_NONE, CT_NONE},
     [0xEA] = {IN_NOP,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xEB] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xEC] = {IN_CMP,  AM_ABS,  RT_X,     RT_NONE, CT_NONE},
     [0xED] = {IN_SBC,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0xEE] = {IN_INC,  AM_ABS,  RT_NONE,  RT_NONE, CT_NONE},
     [0xEF] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},

     // 0xF...    
     [0xF0] = {IN_BCH,  AM_REL,  RT_NONE,  RT_NONE, CT_Z},
     [0xF1] = {IN_SBC,  AM_IDY,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF2] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF3] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF4] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF5] = {IN_SBC,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF6] = {IN_INC,  AM_ZPX,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF7] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xF8] = {IN_FLG,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE}, //SED 
     [0xF9] = {IN_SBC,  AM_ABY,  RT_NONE,  RT_NONE, CT_NONE},
     [0xFA] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xFB] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xFC] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
     [0xFD] = {IN_SBC,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0xFE] = {IN_INC,  AM_ABX,  RT_NONE,  RT_NONE, CT_NONE},
     [0xFF] = {IN_ILL,  AM_IMP,  RT_NONE,  RT_NONE, CT_NONE},
};

instruction *instruction_by_opcode(Byte opcode) {
    return &instructions[opcode];
}

void set_status(cpu_context *ctx, Byte val) {
    // Set N and Z status regs based on val
    if(val == 0) {
        cpu_write_flag(FT_Z, 1);
    } else {
        cpu_write_flag(FT_Z, 0);
    }

    if (val & 0x80) {
        cpu_write_flag(FT_N, 1);
    } else { 
        cpu_write_flag(FT_N, 0);
    }
}

 bool check_cond(cpu_context *ctx) {
    bool z = cpu_read_flag(FT_Z);
    bool c = cpu_read_flag(FT_C);
    bool v = cpu_read_flag(FT_V);
    bool n = cpu_read_flag(FT_N);

    switch(ctx->inst->cond) {
        case CT_NONE: return true;
        case CT_C: return c;
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;
        case CT_N: return n;
        case CT_NN: return !n;
        case CT_V: return v;
        case CT_NV: return !v;
    }

    return false;
}

void proc_none(cpu_context *ctx) {
    printf("INVALID INSTRUCTION!\n");
    exit(-7);
}

void proc_ld(cpu_context *ctx) {
    cpu_write_reg(ctx->inst->reg1, ctx->fetched_data);
    emu_cycles(1);
    set_status(ctx, ctx->fetched_data);
    return;
}

void proc_st(cpu_context *ctx) {
    ctx->fetched_data = cpu_read_reg(ctx->inst->reg1);
    bus_write(ctx->mem_addr, ctx->fetched_data);
    emu_cycles(1);
    return;
}

void proc_reg(cpu_context *ctx) {
    cpu_write_reg(ctx->inst->reg1, cpu_read_reg(ctx->inst->reg2));
    emu_cycles(1);
    return;
}

void proc_stc(cpu_context *ctx) {
    if(ctx->inst->reg2 == RT_STACK) {// PUSH
        Byte stk = cpu_read_reg(RT_STACK);
        Byte dat = cpu_read_reg(ctx->inst->reg1);
        if(ctx->inst->reg1 == RT_ACC) {
            bus_write(0x0100 + stk--, dat);
        } else { // STATUS, set B to 1
            cpu_write_flag(FT_B, 1);
            bus_write(0x0100 + stk--, dat);
        }
        emu_cycles(1);
        cpu_write_reg(ctx->inst->reg2, stk);        
        emu_cycles(1);
        return;
    } else { // PULL
        Byte stk = cpu_read_reg(RT_STACK);
        Byte dat = bus_read(0x0100 + ++stk);        
        if(ctx->inst->reg2 == RT_ACC) {
            cpu_write_reg(ctx->inst->reg2, dat);            
            set_status(ctx, dat);
        } else { // STATUS (PLP)
            cpu_write_reg(ctx->inst->reg2, dat & ~0x10); // Ignoring Break flag
        }
        emu_cycles(1);  
        cpu_write_reg(ctx->inst->reg1, stk);
        emu_cycles(1);
        return;
    }
}

void proc_dec(cpu_context *ctx) {
    if(ctx->inst->reg1 != RT_NONE) { // DEX or DEY
        cpu_write_reg(ctx->inst->reg1, cpu_read_reg(ctx->inst->reg1) - 1);
        set_status(ctx, cpu_read_reg(ctx->inst->reg1));
        emu_cycles(1);
        return;
    } else { // DEC
        Word memAddr = ctx->fetched_data;
        bus_write(memAddr, bus_read(memAddr) - 1);
        set_status(ctx, bus_read(memAddr));
        return;
    }
}

void proc_inc(cpu_context *ctx) {
    if(ctx->inst->reg1 != RT_NONE) { // INX or INY
        ctx->fetched_data = cpu_read_reg(ctx->inst->reg1);
        cpu_write_reg(ctx->inst->reg1, ctx->fetched_data + 1);
        set_status(ctx, cpu_read_reg(ctx->inst->reg1));
        emu_cycles(1);
        return;
    } else { // INC
        bus_write(ctx->mem_addr, ctx->fetched_data + 1);
        set_status(ctx, bus_read(ctx->mem_addr));
        return;
    }
}

void proc_adc(cpu_context *ctx) {
    Byte A = cpu_read_reg(RT_ACC);
    Byte M = ctx->fetched_data;
    Byte status = cpu_read_reg(RT_STATUS);
    unsigned char C = cpu_read_flag(FT_C);

    // Check for overflow FLAG
    // Overflow flag sets if arithmetic on two signed bytes of the same sign
    // will result in bit-7 being high.
    if(((A^(A+M+C)) & (M^(A+M+C))) & 128) {
        cpu_write_flag(FT_V, 1);
    } else { 
        cpu_write_flag(FT_V, 0);
    }

    // Check if carry has to be sets
    if(A > (A + M)) {
        cpu_write_flag(FT_C, 1); 
    } else { 
        cpu_write_flag(FT_C, 0);
    }

    cpu_write_reg(RT_ACC, A + C + M);
    cpu_write_reg(RT_STATUS, status);
    set_status(ctx, A + C + M);
    return;
}

void proc_sbc(cpu_context *ctx) {
    Byte A = cpu_read_reg(RT_ACC);
    Byte M = ctx->fetched_data;
    Byte status = cpu_read_reg(RT_STATUS);
    unsigned char C = cpu_read_flag(FT_C);;

    // Check for overflow FLAG
    // Overflow flag sets if arithmetic on two signed bytes of the same sign
    // will result in bit-7 being high.
    if(((A^(A-M-C)) & (M^(A-M-C))) & 128) {
        cpu_write_flag(FT_V, 1); 
    } else { 
        cpu_write_flag(FT_V, 0); 
    }

    // Check if carry has to be sets [?? Verify if right]
    if(A > (A - M)) {
        cpu_write_flag(FT_C, 1); 
    } else { 
        cpu_write_flag(FT_C, 0);
    }

    cpu_write_reg(RT_ACC, A - C - M);
    cpu_write_reg(RT_STATUS, status);
    set_status(ctx, A - C - M);
    return;

}

void proc_and(cpu_context *ctx) {
    Byte A = cpu_read_reg(RT_ACC);
    A &= ctx->fetched_data;
    cpu_write_reg(RT_ACC, A);
    emu_cycles(1);
    set_status(ctx, A);
    return;
}

void proc_eor(cpu_context *ctx) {
    Byte A = cpu_read_reg(RT_ACC);
    A ^= ctx->fetched_data;
    cpu_write_reg(RT_ACC, A);
    emu_cycles(1);
    set_status(ctx, A);
    return;
}

void proc_ora(cpu_context *ctx) {
    Byte A = cpu_read_reg(RT_ACC);
    A |= ctx->fetched_data;
    cpu_write_reg(RT_ACC, A);
    emu_cycles(1);
    set_status(ctx, A);
    return;
}

void proc_asl(cpu_context *ctx) {
    Byte dat = ctx->fetched_data;
    if(dat & 0x80) { //Set carry if high bit is 1
        cpu_write_flag(FT_C, 1);
    } else {
        cpu_write_flag(FT_C, 0);
    }
    dat = (dat << 1) & 0xFF;
    if(ctx->inst->mode == AM_ACC) {
        cpu_write_reg(RT_ACC, dat);
    } else {
        bus_write(ctx->mem_addr, dat);
    }
    emu_cycles(1);
    set_status(ctx, dat);
    return;
}

void proc_lsr(cpu_context *ctx) {
    Byte dat = ctx->fetched_data;
    Byte status = cpu_read_reg(RT_STATUS);
    if(dat & 0x01) { //Set carry if low bit is 1
        cpu_write_flag(FT_C, 1);
    } else {
        cpu_write_flag(FT_C, 0);
    }
    dat = (dat >> 1) & 0xFF;
    if(ctx->inst->mode == AM_ACC) {
        cpu_write_reg(RT_ACC, dat);
    } else {
        bus_write(ctx->mem_addr, dat);
    }
    emu_cycles(1);
    set_status(ctx, dat);
    return;
}

void proc_rol(cpu_context *ctx) {
    Byte dat = ctx->fetched_data;
    Byte C = cpu_read_flag(FT_C);
    if(dat & 0x80) { //Set carry if high bit is 1
        cpu_write_flag(FT_C, 1);
    } else {
        cpu_write_flag(FT_C, 0);
    }
    dat = (dat << 1) & 0xFF;
    dat += (C & 0xFF);
    if(ctx->inst->mode == AM_ACC) {
        cpu_write_reg(RT_ACC, dat);
    } else {
        bus_write(ctx->mem_addr, dat);
    }
    emu_cycles(1);
    set_status(ctx, dat);
    return;
}

void proc_ror(cpu_context *ctx) {
    Byte dat = ctx->fetched_data;
    Byte C = cpu_read_flag(FT_C);
    if(dat & 0x01) { //Set carry if low bit is 1
        cpu_write_flag(FT_C, 1);
    } else {
        cpu_write_flag(FT_C, 0);
    }
    dat = (dat >> 1) & 0xFF;
    dat += ((C >> 7) & 0xFF);
    if(ctx->inst->mode == AM_ACC) {
        cpu_write_reg(RT_ACC, dat);
    } else {
        bus_write(ctx->mem_addr, dat);
    }
    emu_cycles(1);
    set_status(ctx, dat);
    return;
}

void proc_flg(cpu_context *ctx) {
    switch(ctx->op_code) {
        case 0x18: 
            cpu_write_flag(FT_C, 0);
            emu_cycles(1); 
            return;
        case 0xD8: 
            cpu_write_flag(FT_D, 0); 
            emu_cycles(1);
            return;
        case 0x58: 
            cpu_write_flag(FT_I, 0); 
            // ctx->int_disable = true;
            emu_cycles(1);
            return;
        case 0xB8: 
            cpu_write_flag(FT_V, 0); 
            emu_cycles(1);
            return;
        
        case 0x38: 
            cpu_write_flag(FT_C, 1); 
            emu_cycles(1);
            return;
        case 0xF8: 
            cpu_write_flag(FT_D, 1);
            emu_cycles(1);
            return;
        case 0x78: 
            cpu_write_flag(FT_I, 1); 
            // ctx->int_disable = false;
            emu_cycles(1);
            return;
        default: 
            return; 
    }
}

void proc_cmp(cpu_context *ctx) {
    Byte dat = cpu_read_reg(ctx->inst->reg1);
    Byte M = ctx->fetched_data;
    if(dat == M) {
        cpu_write_flag(FT_Z, 1);
        cpu_write_flag(FT_C, 1);
    } else if(dat > M) {
        cpu_write_flag(FT_Z, 0);
        cpu_write_flag(FT_C, 1);
    } else {
        cpu_write_flag(FT_Z, 0);
        cpu_write_flag(FT_C, 0);
    }
    if(dat >> 7) {
        cpu_write_flag(FT_N, 1);
    } else {
        cpu_write_flag(FT_N, 0);
    }
    return;
}

void proc_bch(cpu_context *ctx) {
    if(check_cond(ctx)) {
        ctx->reg.pc = ctx->mem_addr;
    }
    return;
}

void proc_jmp(cpu_context *ctx) {
    cpu_write_reg(RT_PC, ctx->mem_addr);
    return;
}

void proc_jsr(cpu_context *ctx) {
    Word pc = ctx->reg.pc;
    Byte stk = ctx->reg.stack;
    bus_write(0x0100 + stk--, pc >> 8); // HI
    bus_write(0x0100 + stk--, pc & 0xFF); // LO
    cpu_write_reg(RT_STACK, stk);
    cpu_write_reg(RT_PC, ctx->mem_addr);
    return;
}

void proc_rts(cpu_context *ctx) {
    Word pc;
    Byte stk = ctx->reg.stack;
    pc = bus_read(0x0100 + ++stk);
    pc |= bus_read(0x0100 + ++stk) << 8;
    cpu_write_reg(RT_PC, pc);
    cpu_write_reg(RT_STACK, stk);
    return;
}

void proc_brk(cpu_context *ctx) {
    Word pc = cpu_read_reg(RT_PC);
    emu_cycles(1);
    Byte stk = ctx->reg.stack;
    emu_cycles(1);
    bus_write(0x0100 + stk--, pc >> 8);
    emu_cycles(1);
    bus_write(0x0100 + stk--, pc & 0xFF);
    emu_cycles(1);
    bus_write(0x0100 + stk--, cpu_read_reg(RT_STATUS));  
    emu_cycles(1); 
    cpu_write_reg(RT_STACK, stk);
    emu_cycles(1);
    // Read pc from IRQ vector and set
    cpu_write_reg(RT_PC, bus_read(0xFFFE) | (bus_read(0xFFFF) << 8));
    cpu_write_flag(FT_B, 1);
    // ctx->int_disable = true;
    return;
}

void proc_rti(cpu_context *ctx) {
    Byte stk = cpu_read_reg(RT_STACK);
    emu_cycles(1);
    Byte status = bus_read(0x0100 + ++stk);
    cpu_write_reg(RT_STATUS, status & ~0x10); // Ignoring Break flag
    emu_cycles(1);
    Word pc = bus_read(0x0100 + ++stk);
    emu_cycles(1);
    pc |= bus_read(0x0100 + ++stk) << 8;
    emu_cycles(1);
    cpu_write_reg(RT_STACK, stk);
    emu_cycles(1);
    cpu_write_reg(RT_PC, pc);
    emu_cycles(1);
    return;
}

void proc_bit(cpu_context *ctx) {
    Byte M = ctx->fetched_data;
    Byte A = cpu_read_reg(RT_ACC);   

    cpu_write_flag(FT_N, BIT(M, 7));
    cpu_write_flag(FT_V, BIT(M, 6));

    M &= A;
    if(M==0) cpu_write_flag(FT_Z, 0);
    return;
}

void proc_nop(cpu_context *ctx) {
    emu_cycles(2);
    return;
}

 INST_PROC processors[] = {
    [IN_ILL]  = proc_none,
    // Transfer
    [IN_LD]   = proc_ld,      // LDA, LDX, LDY 
    [IN_ST]   = proc_st,      // STA, STX, STY
    [IN_REG]  = proc_reg,     // TAX, TXA, DEX, INX, TAY, TYA, DAY, INY
    // Stack
    [IN_STC]  = proc_stc,     // TXS,TSX,PHA,PLA,PHP,PLP
    // Decrement & Increment
    [IN_DEC]  = proc_dec,     // DEC, DEX, DEY
    [IN_INC]  = proc_inc,     // INC, INX, INY
    // Arithmetic
    [IN_ADC]  = proc_adc,
    [IN_SBC]  = proc_sbc,
    // Logical
    [IN_AND]  = proc_and,
    [IN_EOR]  = proc_eor,
    [IN_ORA]  = proc_ora,
    // Shift & Rotate
    [IN_ASL]  = proc_asl,
    [IN_LSR]  = proc_lsr,
    [IN_ROL]  = proc_rol,
    [IN_ROR]  = proc_ror,
    // Flag
    [IN_FLG]  = proc_flg,     // CLC, CLD, CLI, CLV, SEC, SED, SEI
    // Comparison
    [IN_CMP]  = proc_cmp,     // CPX, CPY, CMP
    // Branch 
    [IN_BCH]  = proc_bch,     // BCC, BCS, BEQ, BMI, BNE, BPL, BVX, BVS
    // Jump & Subroutine
    [IN_JMP]  = proc_jmp, 
    [IN_JSR]  = proc_jsr,
    [IN_RTS]  = proc_rts,
    // Interrupts
    [IN_BRK]  = proc_brk,
    [IN_RTI]  = proc_rti,
    // Other
    [IN_BIT]  = proc_bit,
    [IN_NOP]  = proc_nop
};

INST_PROC inst_get_process(inst_type type) {
    return processors[type];
}

char *inst_lookup[] = {
    "<NONE>",
    "LD",
    "ST",
    "REG",
    "STC",
    "DEC",
    "INC",
    "ADC",
    "SBC",
    "AND",
    "EOR",
    "ORA",
    "ASL",
    "LSR",
    "ROL",
    "ROR",
    "FLG",
    "CMP",
    "BCH",
    "JMP",
    "JSR",
    "RTS",
    "BRK",
    "RTI",
    "BIT",
    "NOP"
};

char *inst_name(inst_type t) {
    return inst_lookup[t];
}