//
// Created by jq on 2/14/2023.
//

#include <iostream>
#include "ImmGenerator.h"
#include "consts.h"

uint32_t riscv_emu::ImmGenerator::generateImm(ImmType immSel, uint32_t instr) const {
    switch (immSel) {
        case IMM_I_TYPE:
            return getIImm(instr);

        case IMM_S_TYPE:
            return (((instr & Constants::S_IMM_MASK) >> Constants::S_IMM_5_TO_11_SHIFT) << 5) |
                ((instr & Constants::S_IMM_MASK) >> Constants::S_IMM_0_TO_4_SHIFT);

        case IMM_B_TYPE:
            return getBImm(instr);

        case IMM_U_TYPE:
            return getUImm(instr);

        case IMM_J_TYPE:
            return getJImm(instr);


        default:
            break;
    }
}

uint32_t riscv_emu::ImmGenerator::getIImm(uint32_t instr) const {
    if ((((instr & Constants::I_IMM_11_MASK) >> Constants::I_IMM_11_SHIFT) == 0b1)) {
        return Constants::I_IMM_SIGN_EXTEND |
                (instr & Constants::I_IMM_MASK) >> Constants::I_IMM_SHIFT;
    }
    return (instr & Constants::I_IMM_MASK) >> Constants::I_IMM_SHIFT;
}

uint32_t riscv_emu::ImmGenerator::getJImm(uint32_t instr) const {
    if ((((instr & Constants::J_IMM_20_MASK) >> Constants::J_IMM_20_SHIFT) == 0b1)) {
        return Constants::J_IMM_SIGN_EXTEND |
               (((instr & Constants::J_IMM_12_TO_19_MASK) >> Constants::J_IMM_12_TO_19_SHIFT) << 12) |
               (((instr & Constants::J_IMM_1_TO_10_MASK) >> Constants::J_IMM_1_TO_10_SHIFT) << 1) |
               (((instr & Constants::J_IMM_11_MASK) >> Constants::J_IMM_11_SHIFT) << 11);
    }
    return (((instr & Constants::J_IMM_12_TO_19_MASK) >> Constants::J_IMM_12_TO_19_SHIFT) << 12) |
           (((instr & Constants::J_IMM_1_TO_10_MASK) >> Constants::J_IMM_1_TO_10_SHIFT) << 1) |
           (((instr & Constants::J_IMM_11_MASK) >> Constants::J_IMM_11_SHIFT) << 11);
}

uint32_t riscv_emu::ImmGenerator::getBImm(uint32_t instr) const {
    if ((((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_12_SHIFT) == 0b1)) {
        return Constants::B_IMM_SIGN_EXTEND |
               (((instr & Constants::B_IMM_5_TO_10_MASK) >> Constants::B_IMM_5_TO_10_SHIFT) << 5) |
               (((instr & Constants::B_IMM_11_MASK) >> Constants::B_IMM_11_SHIFT) << 11) |
               (((instr & Constants::B_IMM_1_TO_4_MASK) >> Constants::B_IMM_1_TO_4_SHIFT) << 1);
    }
    //return //(((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_11_SHIFT) << 9) |
    return (((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_5_TO_10_SHIFT) << 5) |
           (((instr & Constants::B_IMM_MASK) >> Constants::B_IMM_1_TO_4_SHIFT) << 1);
}


uint32_t riscv_emu::ImmGenerator::getUImm(uint32_t instr) const {
    return instr & Constants::U_IMM_MASK;
}
