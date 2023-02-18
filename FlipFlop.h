//
// Created by jq on 2/17/2023.
//

#ifndef RISCV_EMU_FLIPFLOP_H
#define RISCV_EMU_FLIPFLOP_H


#include <cstdint>
#include "Tickable.h"

namespace riscv_emu {
    class FlipFlop : public Tickable {
    private:
        uint32_t input, output;

    public:
        void tick() override;
        inline uint32_t getOutput() const { return output; }
        inline void setInput(uint32_t val) { input = val; }
    };

}

#endif //RISCV_EMU_FLIPFLOP_H
