//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_IMMGENERATOR_H
#define RISCV_EMU_IMMGENERATOR_H


#include <cstdint>

class ImmGenerator {
private:
    uint32_t getBImm(uint32_t instr);
public:
    uint32_t generateImm(uint32_t instr);
};


#endif //RISCV_EMU_IMMGENERATOR_H
