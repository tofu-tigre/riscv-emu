//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_CONSTS_H
#define RISCV_EMU_CONSTS_H

#include <cstdint>

enum ALU_Mode {
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND
};

enum MemRW {
    READ,
    WRITE,
};

enum WBSelect {
    ALU_SEL,
    MEM_SEL,
    PC_PLUS_4
};

enum PCSel {
    ALU_PC_SEL,
    PC_INCR
};

enum MemMode {
    BYTE,
    HALF_WORD,
    WORD,
    BYTE_UPPER,
    HALF_WORD_UPPER
};
namespace Constants {
    const uint32_t BOOT_ADDR = 0x800;
    const size_t DRAM_SIZE = 4000; // 4 Kilobytes

    const uint32_t OPCODE_MASK = 0b00000000000000000000000001111111;
    const uint32_t R_INSTR =     0b00000000000000000000000000110011;
    const uint32_t I_INSTR =     0b00000000000000000000000000010011;
    const uint32_t I_LOAD_INSTR =0b00000000000000000000000000000011;
    const uint32_t S_INSTR =     0b00000000000000000000000000100011;
    const uint32_t B_INSTR =     0b00000000000000000000000001100011;
    const uint32_t U_INSTR =     0b00000000000000000000000000010111;
    const uint32_t J_INSTR =     0b00000000000000000000000001101111;

    const uint32_t FUNC_7_MASK = 0b11111110000000000000000000000000;
    const uint32_t FUNC_7_SHIFT = 25;
    const uint32_t FUNC_3_MASK = 0b00000000000000000111000000000000;
    const uint32_t FUNC_3_SHIFT = 12;
    const uint32_t RS1_MASK =    0b00000000000011111000000000000000;
    const uint32_t RS1_SHIFT = 15;
    const uint32_t RS2_MASK =    0b00000001111100000000000000000000;
    const uint32_t RS2_SHIFT = 20;
    const uint32_t RD_MASK =     0b00000000000000000000111110000000;
    const uint32_t RD_SHIFT = 7;

    const uint32_t I_IMM_MASK =  0b11111111111100000000000000000000;
    const uint32_t I_IMM_SHIFT = 20;

    const uint32_t S_IMM_MASK =  0b11111110000000000000111110000000;
    const uint32_t S_IMM_0_TO_4_SHIFT = 7;
    const uint32_t S_IMM_5_TO_11_SHIFT = 25;

    const uint32_t B_IMM_MASK =  0b11111110000000000000111110000000;
    const uint32_t B_IMM_1_TO_4_SHIFT = 8;
    const uint32_t B_IMM_11_SHIFT = 7;
    const uint32_t B_IMM_5_TO_10_SHIFT = 25;
    const uint32_t B_IMM_12_SHIFT = 31;

    const uint32_t J_IMM_MASK =         0b11111111111111111111000000000000;
    const uint32_t J_IMM_1_TO_10_MASK = 0b01111111111000000000000000000000;
    const uint32_t J_IMM_20_MASK =      0b10000000000000000000000000000000;
    const uint32_t J_IMM_11_MASK =      0b00000000000100000000000000000000;
    const uint32_t J_IMM_12_TO_19_MASK =0b00000000000011111111000000000000;



     // for func3
    const uint32_t ALU_ADD = 0b000;
    const uint32_t ALU_SUB = 0b000;
    const uint32_t ALU_SLL = 0b001;
    const uint32_t ALU_SLT = 0b010;
    const uint32_t ALU_SLTU = 0b011;
    const uint32_t ALU_XOR = 0b100;
    const uint32_t ALU_SRL = 0b101;
    const uint32_t ALU_SRA = 0b101;
    const uint32_t ALU_OR = 0b110;
    const uint32_t ALU_AND = 0b111;

}

#endif //RISCV_EMU_CONSTS_H
