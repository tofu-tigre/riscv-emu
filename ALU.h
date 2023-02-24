//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_ALU_H
#define RISCV_EMU_ALU_H


#include <cstdint>
#include "consts.h"

namespace riscv_emu {
    /**
     * Represents the ALU component of a CPU. Since the ALU is pure
     * combinatorial logic, it holds no state.
     */
    class ALU {
    private:

    public:
        ALU();

        /**
         * Computes the result of an ALU operation.
         * @param mode - the type of operation to compute.
         * @param a - the left operand of the operation.
         * @param b - the right operand of the operation.
         * @return the result of said operation.
         */
        uint32_t doOp(ALU_Mode mode, uint32_t a, uint32_t b);
    };

}

#endif //RISCV_EMU_ALU_H
