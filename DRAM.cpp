//
// Created by jq on 2/14/2023.
//

#include <algorithm>
#include <cstring>
#include <iostream>
#include "DRAM.h"

riscv_emu::DRAM::DRAM() {
    std::fill_n(this->data, Constants::DRAM_SIZE, 0);
    this->addr = 0;
    this->writeData = 0;
    this->mode = MemMode::WORD;
    this->rwSel = MemRW::READ;
    this->output = 0;
}

riscv_emu::DRAM::DRAM(MemMode mode, MemRW rw, uint32_t addr, uint32_t write) {
    std::fill_n(this->data, Constants::DRAM_SIZE, 0);
    this->mode = mode;
    this->rwSel = rw;
    this->addr = addr;
    this->writeData = write;
    this->output = 0;
}

void riscv_emu::DRAM::read() {
    // TODO: Implement invalid addr. error bits.
    if (addr >= Constants::DRAM_SIZE) {
        error = true;
        return;
    }

    switch (mode) {
        case BYTE:
            output = data[addr];
            break;
        case HALF_WORD:
            output = (data[addr + 1] << 8) | data[addr];
            break;
        case WORD:
            output = (data[addr + 3] << 24) | (data[addr + 2] << 16) | (data[addr + 1] << 8) | data[addr];
            break;
        case HALF_WORD_UPPER:
            output = (data[addr + 3] << 24) | (data[addr + 2] << 16);
            break;
        case BYTE_UPPER:
            output = (data[addr + 3] << 24);
            break;
    }
}

void riscv_emu::DRAM::write() {
    if (addr >= Constants::DRAM_SIZE) {
        error = true;
        return;
    }

    switch (mode) {
        case BYTE:
            data[addr] = writeData & 0x000000FF;
            break;
        case HALF_WORD:
            data[addr] = writeData & 0x000000FF;
            data[addr + 1] = (writeData & 0x0000FF00) >> 8;
            break;
        case WORD:
            data[addr] = writeData & 0x000000FF;
            data[addr + 1] = (writeData & 0x0000FF00) >> 8;
            data[addr + 2] = (writeData & 0x00FF0000) >> 16;
            data[addr + 3] = (writeData & 0xFF000000) >> 24;
            break;
    }
}

/**
 * Loads a program beginning at memory address addr.
 * @param addr - address in memory at which program begins.
 * @param program - pointer to beginning of program.
 * @param size - number of bytes in the program to load.
 */
void riscv_emu::DRAM::load(size_t addr, uint32_t *program, size_t size) {
    if (addr + size >= Constants::DRAM_SIZE) {
        error = true;
        return;
    }

    std::memcpy(&this->data[addr], program, size);
}

void riscv_emu::DRAM::tick() {
    switch (rwSel) {
        case MemRW::WRITE:
            write();
            return;
        case MemRW::READ:
            return;
    }

}

uint32_t riscv_emu::DRAM::getOutput() {
    switch (rwSel) {
        case READ:
            read();
            return output;
        case WRITE:
            return output;
    }
}

