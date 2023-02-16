//
// Created by jq on 2/14/2023.
//

#include <iostream>
#include <cstring>
#include <bitset>
#include "ALU.h"

ALU::ALU() {
    this->overflow = false;
    this->last = 0;
}

uint32_t ALU::doOp(ALU_Mode mode, uint32_t a, uint32_t b) {
    int32_t result;

    int32_t at;
    std::memcpy(&at, &a, sizeof(at));

    int32_t bt;
    std::memcpy(&bt, &b, sizeof(bt));

    switch(mode) {
        case ADD:
            std::cout << "ALU op: add\n";
            // Test for overflow
            if (b > 0 && a > (UINT32_MAX - b)) {
                this->overflow = true;
            } else {
                this->overflow = false;
            }


            1000 0000 0000 1111 1111 1111 1111 1100
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
    uint32_t out;
    std::memcpy(&out, &result, sizeof(out));
    this->last = out;
    return out;
}

bool ALU::isOverflow() {
    return this->overflow;
}

uint32_t ALU::getLastResult() {
    return this->last;
}

int32_t ALU::twos(uint32_t val) {
    if ((val >> 31) == 0b0) {
        return (int32_t) val;
    }

    return 0;
}

