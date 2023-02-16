//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_ALU_H
#define RISCV_EMU_ALU_H


#include <cstdint>
#include "consts.h"

class ALU {
private:
    bool overflow;
    uint32_t last;

    int32_t twos(uint32_t val);

public:
    ALU();
    uint32_t doOp(ALU_Mode mode, uint32_t a, uint32_t b);
    bool isOverflow();
    uint32_t getLastResult();
};


#endif //RISCV_EMU_ALU_H
