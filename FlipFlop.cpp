//
// Created by jq on 2/17/2023.
//

#include "FlipFlop.h"

void riscv_emu::FlipFlop::tick() {
    output = input;
}
