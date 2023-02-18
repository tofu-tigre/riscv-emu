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
            std::cout << "ALU op: add\n";
            result = at + bt;
            break;

        case SUB:
            std::cout << "ALU op: sub\n";
            result = at - bt;
            break;

        case SLL:
            std::cout << "ALU op: sll\n";
            result = at << bt;
            break;

        case SLT:
            std::cout << "ALU op: slt\n";
            if (at < bt) {
                result = 1;
                break;
            }
            result = 0;
            break;

        case SLTU:
            std::cout << "ALU op: sltu\n";
            if (a < b) {
                result = 1;
                break;
            }
            result = 0;
            break;

        case XOR:
            std::cout << "ALU op: xor\n";
            result = at ^ bt;
            break;

        case SRL:
            std::cout << "ALU op: srl\n";
            result = at >> bt;
            break;

        case SRA:
            std::cout << "ALU op: sra\n";
            result = at >> bt;
            break;

        case OR:
            std::cout << "ALU op: or\n";
            result = at | bt;
            break;

        case AND:
            std::cout << "ALU op: and\n";
            result = at & bt;
            break;
    }

    uint32_t output;
    std::memcpy(&output, &result, sizeof(output));
    return output;
}
