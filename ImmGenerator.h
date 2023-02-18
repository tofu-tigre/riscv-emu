//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_IMMGENERATOR_H
#define RISCV_EMU_IMMGENERATOR_H


#include <cstdint>
#include "consts.h"

namespace riscv_emu {
    class ImmGenerator {
    private:
        uint32_t getBImm(uint32_t instr) const;
        uint32_t getJImm(uint32_t instr) const;
        uint32_t getIImm(uint32_t instr) const;
        uint32_t getUImm(uint32_t instr) const;

    public:
        uint32_t generateImm(ImmType immSel, uint32_t instr) const;
    };
}


#endif //RISCV_EMU_IMMGENERATOR_H
