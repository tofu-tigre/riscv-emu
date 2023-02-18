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
    class CPU {
    private:
        FlipFlop pc;
        uint32_t clock;
        Controller controller;
        RegisterArray registers;
        ALU alu;
        ImmGenerator immGenerator;
        BranchComp branchComp;

        // Register selects:
        uint8_t rs1;
        uint8_t rs2;
        uint8_t rd;
        uint32_t imm; // Output of the immediate generator.
        //uint32_t instr;
        uint32_t aluOut;
        uint32_t wbData;

        bool running;

        DRAM instrMem;
        DRAM dataMem;

        // Pipelines vars
        FlipFlop fetchInstr, decodeInstr, executeInstr, memInstr, wbInstr;


        void fetch();
        void decode();
        void execute();
        void memory();
        void writeBack();
        void run();
        void tick();

    public:
        CPU();
        void boot(uint32_t* program, size_t size);
    };

}

#endif //RISCV_EMU_CPU_H
