#include <iostream>
#include "CPU.h"


int main() {

    /*
     * HOW TO USE:
     * Simply create an array of uint32_t RISC-V 32I instructions and
     * input the array pointer into the vm.boot function. Below,
     * I have provided some example programs.
     * Note: To signify the end of a program, use the exit instruction
     * 0xff.
     */

    // Computes 2 ^ x power using recursion.
    uint32_t testProgram[19] = {
            0x0ff00113, //          addi x2, x0, 0xFF  # Set the stack pointer up               800
            0x00811113, //          slli x2, x2, 8                                              804
            0x0fc10113, //          addi x2, x2 0xfc  # Set the stack pointer up                808

            0x00600513, //          addi x10, x0, 5  # arg1 = 5                                 80c
            0x00100593, //          addi x11, x0, 2  # arg2 = 1                                 810
            0x00c000ef, //          jal x1, 12                                                  814
            0x000582b3, //          add x5, x10, x0   # return the value of func1               818
            0xff,       //          exit                                                        81c

            0xffc10113, // dive:    addi x2, x2, -4   # Move the stack pointer down             820
            0x00112023, //          sw x1, 0(x2)      # Save the return address                 824
            0x00050863, //          beq x10, x0, 16                                            828
            0xfff50513, //          addi x10, x10, -1 # Decrement the loop                     82c
            0x00159593, //          slli x11, x11, 1 # Multiply by 2                           830
            0xfedff0ef, //          jal x1, -20       # Jump back to dive                      834

            0x00012083,//          lw x1, 0(x2)      # Restore the return address              838
            0x00410113,//          addi x2, x2, 4    # Restore the stack pointer               83c
            0x00008067//          jalr x0, x1, 0    # Return                                   840
    };


    // JALR WORKS
    uint32_t testJALRHazard[10] = {
            0x00c00093, // addi x1, x0, 12      800
            0x40000193, // addi x3, x0, 0x400   804
            0x42018193, // addi x3, x3, 0x420   808
            0x00018067, // jalr x0, x3, 0       80c
            0x00500093, // addi x1, x0, 5       810
            0x00300093, // addi x1, x0, 3       814
            0x00500093, // addi x1, x0, 5       818
            0x00300093, // addi x1, x0, 3       81c
            0x02200113, // addi x2, x0, 34      820
            0xff

    };


    // JAL WORKS
    uint32_t testJALHazard[8] = {
            0x00c00093, // addi x1, x0, 12      800
            0x0140006f, // jal x0, 20           804
            0x00500093, // addi x1, x0, 5       808
            0x00300093, // addi x1, x0, 3       80c
            0x00500093, // addi x1, x0, 5       810
            0x00300093, // addi x1, x0, 3       814
            0x02200113, // addi x2, x0, 34      818
            0xff

    };

    uint32_t testBEQHazard[8] = {
            0x00000093, // addi x1, x0, 12      800
            0x00008a63, // beq x1, x0 20        804
            0x00500093, // addi x1, x0, 5       808
            0x00300093, // addi x1, x0, 3       80c
            0x00500093, // addi x1, x0, 5       810
            0x00300093, // addi x1, x0, 3       814
            0x04000113, // addi x2, x0, 64      818
            0xff

    };

    uint32_t testRAWHazard[7] = {
            0x04000113, // addi x2, x0, 64
            0x02000093, // addi x1, x0, 32
            0x401101b3, // sub x3, x2, x1
            0x0021a023, // sw x2, 0(x3)
            0x00111113, // slli x2, x2, 1
            0x0001a103, // lw x2, 0(x3)
            0xff
    };

    riscv_emu::CPU vm = riscv_emu::CPU();
    vm.boot(testProgram, 20 * sizeof(uint32_t));
    return 0;
}
