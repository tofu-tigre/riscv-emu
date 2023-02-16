#include <iostream>
#include "CPU.h"

int main() {
    uint32_t testProgram[7] = {
            0x00a00093, // addi x1, x0, 10
            0x00110263, // beq x2, x1, end
            0x00110113, // addi x2 x2 1
            0xfffff06f, // jal x0, start
            0xFF,
            0xFF

    };

    CPU vm = CPU(testProgram);
    vm.tick();
    return 0;
}
