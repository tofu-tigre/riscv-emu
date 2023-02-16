//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_IMMGENERATOR_H
#define RISCV_EMU_IMMGENERATOR_H


#include <cstdint>

class ImmGenerator {
public:
    uint32_t generateImm(uint32_t instr);
};


#endif //RISCV_EMU_IMMGENERATOR_H
