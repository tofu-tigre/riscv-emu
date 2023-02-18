//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_ALU_H
#define RISCV_EMU_ALU_H


#include <cstdint>
#include "consts.h"

namespace riscv_emu {
    class ALU {
    private:

    public:
        ALU();
        uint32_t doOp(ALU_Mode mode, uint32_t a, uint32_t b);
    };

}

#endif //RISCV_EMU_ALU_H
