//
// Created by jq on 2/17/2023.
//

#ifndef RISCV_EMU_TICKABLE_H
#define RISCV_EMU_TICKABLE_H

namespace riscv_emu {
    class Tickable {
    public:
        virtual void tick() = 0;
    };
}



#endif //RISCV_EMU_TICKABLE_H
