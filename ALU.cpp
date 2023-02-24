//
// Created by jq on 2/14/2023.
//

#include <iostream>
#include <cstring>
#include <bitset>
#include "ALU.h"

riscv_emu::ALU::ALU() {
}

uint32_t riscv_emu::ALU::doOp(ALU_Mode mode, uint32_t a, uint32_t b) {
    int32_t result;

    int32_t at;
    std::memcpy(&at, &a, sizeof(at));
    int32_t bt;
    std::memcpy(&bt, &b, sizeof(bt));

    switch(mode) {
        case B:
            result = bt;
            break;
        case ADD:
            result = at + bt;
            break;

        case SUB:
            result = at - bt;
            break;

        case SLL:
            result = at << bt;
            break;

        case SLT:
            if (at < bt) {
                result = 1;
                break;
            }
            result = 0;
            break;

        case SLTU:
            if (a < b) {
                result = 1;
                break;
            }
            result = 0;
            break;

        case XOR:
            result = at ^ bt;
            break;

        case SRL:
            result = at >> bt;
            break;

        case SRA:
            result = at >> bt;
            break;

        case OR:
            result = at | bt;
            break;

        case AND:
            result = at & bt;
            break;
    }

    uint32_t output;
    std::memcpy(&output, &result, sizeof(output));
    return output;
}
