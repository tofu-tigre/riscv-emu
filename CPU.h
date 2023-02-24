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
#include "RegisterArray.h"
#include "FlipFlop.h"

namespace riscv_emu {
    /**
     * A RISC-V CPU. Contains all components in a traditional
     * 5-stage pipelined CPU.
     */
    class CPU {
    private:
        uint32_t clock;
        ALU alu;
        ImmGenerator immGenerator;
        BranchComp branchComp;
        bool running;
        RegisterArray registers;
        DRAM instrMem;
        DRAM dataMem;

        // Pipelines vars
        Controller controller;
        FlipFlop decodeInstr, executeInstr, memInstr, wbInstr;
        FlipFlop pc1, pc2, pc3, pc4, pc5;
        FlipFlop rd1, rd2, md1, md2, wbData, aluOut;


        void fetch();
        void decode();
        void execute();
        void memory();
        void writeBack();
        void run();
        void tick();
        void interlock();

    public:
        CPU();
        void boot(uint32_t* program, size_t size);
    };

}

#endif //RISCV_EMU_CPU_H
