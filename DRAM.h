//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_DRAM_H
#define RISCV_EMU_DRAM_H


#include <cstdint>
#include "consts.h"

class DRAM {
private:
    uint32_t data[Constants::DRAM_SIZE];

public:
    DRAM();
    void load(size_t addr, uint32_t* program, size_t size);
    uint32_t read(size_t addr, MemMode mode);
    void write(size_t addr, uint32_t val, MemMode mode);

};


#endif //RISCV_EMU_DRAM_H
