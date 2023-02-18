#include <iostream>
#include "CPU.h"

int main() {
    uint32_t testProgram[20] = {
            0x0ff00113, //          addi x2, x0, 0xFF  # Set the stack pointer up
            0x00811113, //          slli x2, x2, 8
            0x0fc10113, //          addi x2, x2 0xfc  # Set the stack pointer up

            0x00500513, //          addi x10, x0, 6  # arg1 = 20
            0x00100593, //          addi x11, x0, 2
            0x010000ef, //          jal x1, 12
            0x000502b3, //          add x5, x10, x0   # return the value of func1
            0x4ffff4b7,
            0xff,       //          exit

            0xffc10113, // dive:    addi x2, x2, -4   # Move the stack pointer down
            0x00112023, //          sw x1, 0(x2)      # Save the return address
            0x00050863, //          beq x10, x0, 16
            0xfff50513, //          addi x10, x10, -1 # Decrement the loop
            0x00159593, //          slli x11, x11, 1 # Multiply by 2
            0xfedff0ef, //          jal x1, -20       # Jump back to dive

            0x00012083,//          lw x1, 0(x2)      # Restore the return address
            0x00410113,//          addi x2, x2, 4    # Restore the stack pointer
            0x00008067//          jalr x0, x1, 0    # Return
    };

    uint32_t simple[4] = {
            0x00c00093, // addi x1, x0, 12
            0x00800113, // addi x2, x0, 8
            0x402081b3,  // sub x3, x1, x2
            0xFF
    };

    riscv_emu::CPU vm = riscv_emu::CPU();
    vm.boot(testProgram, 20 * sizeof(uint32_t));
    return 0;
}
