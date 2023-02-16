//
// Created by jq on 2/14/2023.
//

#include <iostream>
#include "ImmGenerator.h"
#include "consts.h"

uint32_t ImmGenerator::generateImm(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;

    switch (opcode) {
        case Constants::I_LOAD_INSTR:
            return (instr & Constants::I_IMM_MASK) >> Constants::I_IMM_SHIFT;

        case Constants::I_INSTR:
            return (instr & Constants::I_IMM_MASK) >> Constants::I_IMM_SHIFT;

        case Constants::S_INSTR:
            return (((instr & Constants::S_IMM_MASK) >> Constants::S_IMM_5_TO_11_SHIFT) << 5) |
                ((instr & Constants::S_IMM_MASK) >> Constants::S_IMM_0_TO_4_SHIFT);

        case Constants::B_INSTR:
            return (((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_12_SHIFT) << 12) |
                    (((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_5_TO_10_SHIFT) << 5) |
                    (((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_11_SHIFT) << 11) |
                    (((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_1_TO_4_SHIFT) << 1);
            break;

        case Constants::U_INSTR:
            break;

        case Constants::J_INSTR:
            std::cout << "J-TYPE IMM:\n";
            return (((instr & Constants::J_IMM_20_MASK) >> 31) << 31) |
                   (((instr & Constants::J_IMM_12_TO_19_MASK) >> (5 + 7)) << 12) |
                   (((instr & Constants::J_IMM_1_TO_10_MASK) >> (7 + 5 + 8 + 1)) << 1) |
                   (((instr & Constants::J_IMM_11_MASK) >> (7 + 5 + 8)) << 11);

        default:
            break;
    }
}
