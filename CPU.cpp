//
// Created by jq on 2/14/2023.
//

#include <algorithm>
#include <iostream>
#include "CPU.h"
#include "consts.h"

CPU::CPU(uint32_t* progAddr) {
    this->pc = Constants::BOOT_ADDR;
    this->clock = 0;
    std::fill_n(this->registers, 32, 0);
    this->instrMem = DRAM();
    this->dataMem = DRAM();
    this->controller = Controller();
    this->alu = ALU();
    this->immGenerator = ImmGenerator();

    this->instrMem.load(Constants::BOOT_ADDR, progAddr, 7  * sizeof(uint32_t));
}

void CPU::tick() {
    while(true) {
        // FETCH
        switch (this->controller.getPCSel()) {
            case ALU_PC_SEL:
                this->pc = this->alu.getLastResult();
                break;
            case PC_INCR:
                this->pc = this->pc;
                break;
            default:
                std::cout << "ERROR\n";
                break;
        }

        uint32_t instr = this->fetch();
        // Decode the next instr.
        std::cout << "Clock: " << this->clock << "\tInstruction address: " << this->pc << "\tCurrent instruction: " << instr << std::endl;

        if (instr == 0xFF) {
            // Kill VM.
            // print what is in registers x0 - x10
            std::cout << "\nRegister status:\n";
            for (int i = 0; i < 32; i++) {
                std::cout << "x" << i << ": " << this->registers[i] << std::endl;
            }
            return;
        }

        // Decode
        this->controller.setFlags(instr);
        if(this->controller.isInvalid()) {
            std::cout << "ERROR: Invalid opcode.\n";
            break;
        }

        uint32_t imm = this->immGenerator.generateImm(instr);

        this->rs1 = (instr & Constants::RS1_MASK) >> Constants::RS1_SHIFT;
        this->rs2 = (instr & Constants::RS2_MASK) >> Constants::RS2_SHIFT;
        this->rd = (instr & Constants::RD_MASK) >> Constants::RD_SHIFT;

        uint32_t alu_a = this->registers[this->rs1];
        uint32_t alu_b = this->registers[this->rs2];
        uint32_t aluOut;

        if (this->controller.getASel()) {
            alu_a = this->pc;
            std::cout << "rs1: pc\n";
        } else {
            // Print out the selected register
            std::cout << "rs1: x" << (int)this->rs1 << "\tvalue: " << this->registers[this->rs1] << "\n";
        }

        if (this->controller.getBSel()) { // select the immediate.
            alu_b = imm;
            std::cout << "imm: " << (int)imm << "\n";
        } else {
            std::cout << "rs2: x" << (int)this->rs2 << "\tvalue: " << this->registers[this->rs2] << "\n";
        }

        std::cout << "rd: " << (int)this->rd << "\n";

        aluOut = this->alu.doOp(this->controller.getALUMode(), alu_a, alu_b);
        std::cout << "ALU output: " << aluOut << "\n";

        // Get addr from mem.
        uint32_t memData = 0;

        // MEM STAGE
        if (this->controller.getMemRW() == MemRW::WRITE) {
            this->dataMem.write(aluOut, this->registers[this->rs2], this->controller.getMemMode());
        } else {
            memData = this->dataMem.read(aluOut, this->controller.getMemMode());
        }

        if (this->controller.getRegWEn() && this->rd != 0x00) {
            std::cout << "Writing back\n";
            switch (this->controller.getWBSel()) {
                case ALU_SEL:
                    this->registers[this->rd] = aluOut;
                    break;
                case MEM_SEL:
                    this->registers[this->rd] = memData;
                    break;
                case PC_PLUS_4:
                    this->registers[this->rd] = this->pc + 1;
                    break;

            }
        }


        this->clock++;
        this->pc++;
    }
}

uint32_t CPU::fetch() {
    // Fetch the next instr.
    return this->instrMem.read(this->pc, MemMode::WORD);
}

uint32_t CPU::getReg(uint8_t reg) {
    return this->registers[reg];
}
