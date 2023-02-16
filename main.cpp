#include <iostream>
#include "CPU.h"

int main() {
    uint32_t testProgram[7] = {
            0x01000093, // addi x1, x0, 16
            0x00208133, // addi x2, x0, 32
            0x00000033, // sw x1, 0(x2)
            0x00000033, // lw x3, 0(x2)
            0x00000033, // slli x4, x3, 2
            0xffdff06f, // jal x0 -4
            0xFF

    };

    CPU vm = CPU(testProgram);
    vm.tick();
    return 0;
}
