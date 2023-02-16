//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_CPU_H
#define RISCV_EMU_CPU_H

#include <cstdint>
#include "DRAM.h"
#include "Controller.h"
#include "ALU.h"
#include "ImmGenerator.h"
#include "BranchComp.h"


class CPU {
private:
    uint32_t pc;
    uint32_t clock;
    uint32_t registers[32];

    // Control flags:
    Controller controller;
    ALU alu;
    ImmGenerator immGenerator;
    BranchComp branchComp;

    // Register selects:
    uint8_t regWrite;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t rd;
    uint32_t immOut; // Output of the immediate generator.



    DRAM instrMem;
    DRAM dataMem;

    uint32_t fetch();
    uint32_t getReg(uint8_t reg);

public:
    CPU(uint32_t* progAddr);
    void tick();
};


#endif //RISCV_EMU_CPU_H
