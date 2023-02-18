//
// Created by jq on 2/17/2023.
//

#include <algorithm>
#include "RegisterArray.h"

riscv_emu::RegisterArray::RegisterArray() {
    std::fill_n(registers, 32, 0);
    rs1 = 0;
    rs2 = 0;
    rd = 0;
    dataWrite = 0;
    writeEnable = false;
    error = false;
}

void riscv_emu::RegisterArray::tick() {
    if (writeEnable && rd != 0) {
        registers[rd] = dataWrite;
    }
}
