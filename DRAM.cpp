//
// Created by jq on 2/14/2023.
//

#include <algorithm>
#include <cstring>
#include <iostream>
#include "DRAM.h"

DRAM::DRAM() {
    std::fill_n(this->data, Constants::DRAM_SIZE, 0);
}

uint32_t DRAM::read(size_t addr, MemMode mode) {
    // TODO: Implement invalid addr. error bits.
    std::cout << "READING FROM ADDR " << addr << std::endl;
    if (addr >= Constants::DRAM_SIZE) {
        //return 0;
    }

    switch (mode) {
        case BYTE:
            return this->data[addr] & 0x00000011;
        case HALF_WORD:
            return this->data[addr] & 0x00001111;
        case WORD:
            return this->data[addr];
        case HALF_WORD_UPPER:
            return this->data[addr] & 0x11110000;
        case BYTE_UPPER:
            return this->data[addr] & 0x11000000;
        default:
            std::cout << "ERROR\n";
    }
}

void DRAM::write(size_t addr, uint32_t val, MemMode mode) {
    std::cout << "WRITING " << val << " TO ADDR " << addr << std::endl;
    if (addr >= Constants::DRAM_SIZE) {
        //return;
    }

    switch (mode) {
        case BYTE:
            this->data[addr] = val & 0x00000011;
            break;
        case HALF_WORD:
            this->data[addr] = val & 0x00001111;
            break;
        case WORD:
            this->data[addr] = val;
            break;
        default:
            std::cout << "ERROR\n";
            break;
    }
}

/**
 * Loads a program beginning at memory address addr.
 * @param addr - address in memory at which program begins.
 * @param program - pointer to beginning of program.
 * @param size - number of bytes in the program to load.
 */
void DRAM::load(size_t addr, uint32_t *program, size_t size) {
    // TODO: Check that memory can store the program.
    std::memcpy(&this->data[addr], program, size);
}
