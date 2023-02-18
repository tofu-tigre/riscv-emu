//
// Created by jq on 2/14/2023.
//

#include <algorithm>
#include <iostream>
#include <cstring>
#include "CPU.h"
#include "consts.h"

riscv_emu::CPU::CPU() {
    this->pc = FlipFlop();
    pc.setInput(Constants::BOOT_ADDR);
    pc.tick();
    this->running = true;
    this->clock = 0;
    this->aluOut = 0;

    this->instrMem = DRAM(MemMode::WORD, MemRW::READ, Constants::BOOT_ADDR, 0);
    this->dataMem = DRAM();
    this->controller = Controller();
    this->alu = ALU();
    this->branchComp = BranchComp();
    this->immGenerator = ImmGenerator();
    this->registers = RegisterArray();
    this->aluOut = 0;
    this->wbData = 0;

    // Load our test program into the instruction memory.

}

void riscv_emu::CPU::boot(uint32_t *program, size_t size) {
    instrMem.load(Constants::BOOT_ADDR, program, size);
    tick();
}


void riscv_emu::CPU::tick() {
    while(this->running) {
        // FETCH
        fetch();
        // Decode the next instr.

        std::cout << "Clock: " << this->clock << "\tPC: " << pc.getOutput() << "\tSP: " << registers.get(2) << "\tCurrent instruction: " << std::hex << instr << std::endl;
        std::cout.copyfmt(std::ios(NULL));

        if (instr == 0xFF) {
            // Kill VM.
            // print what is in registers x0 - x10
            instr = 0x00000033;
            running = false;
        }

        decode();

        if(this->controller.isInvalid()) {
            std::cout << "ERROR: Invalid opcode.\n";
            this->running = false;
            break;
        }


        std::cout << "Opcode: " << controller.getOpcodeString() << std::endl;
        std::cout << "rs1: x" << (int)rs1 << "\trs2: x" << (int)rs2 << "\trd: x" << (int)rd << std::endl;

        execute();
        memory();
        writeBack();


        this->clock++;
        pc.setInput(pc.getOutput() + 4);
    }

    std::cout << "\nRegister status:\n";
    for (int i = 0; i < 32; i++) {
        std::cout << "x" << i << ": " << registers.get(i) << std::endl;
    }

}

void riscv_emu::CPU::fetch() {
    // Fetch the next instr.
    switch (controller.getPCSel()) {
        case ALU_PC_SEL:
            pc.setInput(aluOut);
            break;
        case PC_INCR:
            pc.setInput(pc.getOutput() + 4);
            break;
        default:
            this->running = false;
            break;
    }

    pc.tick();
    instrMem.setInput(MemMode::WORD, MemRW::READ, pc.getOutput(), 0);
    instrMem.tick();
    fetchInstr.setInput(instrMem.getOutput());
}

void riscv_emu::CPU::decode() {
    // Decode
    uint32_t instr = instrMem.getOutput();
    controller.setFlags(instr);

    imm = immGenerator.generateImm(controller.getImmSel(), instr);
    int32_t at;
    std::memcpy(&at, &imm, sizeof(at));

    std::cout << "Imm: " << at << std::endl;

    rs1 = (instr & Constants::RS1_MASK) >> Constants::RS1_SHIFT;
    rs2 = (instr & Constants::RS2_MASK) >> Constants::RS2_SHIFT;
    rd = (instr & Constants::RD_MASK) >> Constants::RD_SHIFT;

    // Tick the register array
    // TODO: Temporary fix. Must update with better solution.
    registers.tick();
    registers.setInput(rs1, rs2, rd, controller.getRegWEn(), wbData);

    // Feed reg info into branch comp
    bool unsgn = controller.getBrUn();
    bool regEq = this->branchComp.equal(unsgn, registers.getRs1Out(), registers.getRs2Out());
    bool regLt = this->branchComp.lessThan(unsgn, registers.getRs1Out(), registers.getRs2Out());
    controller.setBranch(instr, regEq, regLt);
    decodeInstr.setInput(instr);
}

void riscv_emu::CPU::memory() {
    uint32_t instr = executeInstr.getOutput();
    // MEM STAGE
    dataMem.tick();
    dataMem.setInput(controller.getMemMode(), controller.getMemRW(), aluOut, registers.getRs2Out());
    wbInstr.setInput(instr);
}

void riscv_emu::CPU::writeBack() {
    uint32_t instr = memInstr.getOutput();
    switch (this->controller.getWBSel()) {
        case ALU_SEL:
            wbData = aluOut;
            break;
        case MEM_SEL:
            wbData = dataMem.getOutput();
            break;
        case PC_PLUS_4:
            wbData = pc.getOutput() + 4;
            break;
    }
    registers.setWriteData(wbData);
    wbInstr.setInput(instr);
}

void riscv_emu::CPU::execute() {
    uint32_t instr = decodeInstr.getOutput();

    uint32_t alu_a = registers.getRs1Out();
    uint32_t alu_b = registers.getRs2Out();

    if (this->controller.getASel()) {
        alu_a = pc.getOutput();
    }

    if (this->controller.getBSel()) { // select the immediate.
        alu_b = imm;
    }

    aluOut = alu.doOp(this->controller.getALUMode(), alu_a, alu_b);
    executeInstr.setInput(instr);
}

void riscv_emu::CPU::run() {

}

