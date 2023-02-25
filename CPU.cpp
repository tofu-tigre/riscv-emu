//
// Created by jq on 2/14/2023.
//

#include <algorithm>
#include <iostream>
#include <cstring>
#include "CPU.h"
#include "consts.h"

#define DEBUG

riscv_emu::CPU::CPU() {
    this->pc1 = FlipFlop();
    this->pc2 = FlipFlop();
    this->pc3 = FlipFlop();
    this->pc4 = FlipFlop();
    this->pc5 = FlipFlop();

    this->decodeInstr = FlipFlop();
    decodeInstr.setInput(Constants::BUBBLE);
    this->executeInstr = FlipFlop();
    executeInstr.setInput(Constants::BUBBLE);
    this->memInstr = FlipFlop();
    memInstr.setInput(Constants::BUBBLE);
    this->wbInstr = FlipFlop();
    wbInstr.setInput(Constants::BUBBLE);

    this->rd1 = FlipFlop();
    this->rd2 = FlipFlop();
    this->aluOut = FlipFlop();
    this->wbData = FlipFlop();
    this->md1 = FlipFlop();
    this->md2 = FlipFlop();
    this->md3 = FlipFlop();


    pc1.setInput(Constants::BOOT_ADDR);
    this->running = true;
    this->clock = 0;

    this->dmem = DRAM(MemMode::WORD, MemRW::READ, Constants::BOOT_ADDR, 0);
    this->instrCache = new DirectMapCache(&dmem);
    this->dataCache = new DirectMapCache(&dmem);
    this->controller = Controller();
    this->alu = ALU();
    this->branchComp = BranchComp();
    this->immGenerator = ImmGenerator();
    this->registers = RegisterArray();

    tick();
}

void riscv_emu::CPU::boot(uint32_t *program, uint32_t* data, size_t iSize, size_t dSize) {
    dmem.load(Constants::BOOT_ADDR, Constants::DATA_ADDR, program, data, iSize, dSize);
    run();
}

void riscv_emu::CPU::fetch() {
    // Fetch the next instr.

    /*
     * If we stopped running (either encountered an invalid opcode or
     * the exit instruction), continue incrementing the pc like
     * normal but do not fetch more instructions from IMEM, just bubble.
     */
    if (!running) {
        pc1.setInput(pc1.getOutput() + 4);
        decodeInstr.setInput(Constants::BUBBLE);
        pc2.setInput(pc1.getOutput());
        return;
    }

    pc1.setInput(pc1.getOutput() + 4);
    //instrMem.setInput(MemMode::WORD, MemRW::READ, pc1.getOutput(), 0);

    decodeInstr.setInput(instrCache->read(CacheByteSelect::CACHE_SEL_WORD, pc1.getOutput()));

    pc2.setInput(pc1.getOutput());
}

void riscv_emu::CPU::decode() {
    // Decode the instruction at the current PC of the Fetch stage.
    uint32_t instr = decodeInstr.getOutput();

    // Catch our exit code.
    if (instr == Constants::EXIT_CODE || !running) {
        // Kill VM.
        // print what is in registers x0 - x10
        instr = Constants::BUBBLE;
        running = false;
    }

    controller.setDecodeFlags(instr);

    // Generate the immediate from the current instruction.
    uint32_t imm = immGenerator.generateImm(controller.getImmSel(), instr);

    // Select the registers from the instruction bits.
    uint32_t rs1 = (instr & Constants::RS1_MASK) >> Constants::RS1_SHIFT;
    uint32_t rs2 = (instr & Constants::RS2_MASK) >> Constants::RS2_SHIFT;

    // Set rs1, rs2, and rd such that register memory outputs their data.
    registers.setRs1(rs1);
    registers.setRs2(rs2);

    // Set the inputs of the flip-flops to the execute phase.
    rd1.setInput(registers.getRs1Out());
    rd2.setInput(registers.getRs2Out());
    md1.setInput(registers.getRs2Out());
    if (controller.getASel()) {
        rd1.setInput(pc2.getOutput());
    }
    if (controller.getBSel()) {
        rd2.setInput(imm);
    }

    // Set the input of the instruction flip-flop to the execute phase.
    executeInstr.setInput(instr);
    pc3.setInput(pc2.getOutput());


}

void riscv_emu::CPU::execute() {
    // Get the instruction from the previous phase.
    uint32_t instr = executeInstr.getOutput();

    /*
     * Branch decision is made in the beginning of the execute phase.
     * We must select the registers from the register array since
     * the flip-flops from the previous phase may contain incorrect values.
     */
    uint32_t rs1 = (instr & Constants::RS1_MASK) >> Constants::RS1_SHIFT;
    uint32_t rs2 = (instr & Constants::RS2_MASK) >> Constants::RS2_SHIFT;

    bool unsgn = controller.getBrUn();
    bool regEq = branchComp.equal(unsgn, registers.get(rs1), registers.get(rs2));
    bool regLt = branchComp.lessThan(unsgn, registers.get(rs1), registers.get(rs2));

    // Set the proper control flags for the execute stage.
    controller.setBranch(instr, regEq, regLt);
    controller.setExecuteFlags(instr);

    // Set the flip-flop for the alu out and memory stage.
    uint32_t result = alu.doOp(controller.getALUMode(), rd1.getOutput(), rd2.getOutput());
    aluOut.setInput(result);
    memInstr.setInput(instr);
    md2.setInput(md1.getOutput());
    pc4.setInput(pc3.getOutput());

    /*
     * Check if the current stage's instruction was a JUMP.
     * We check here instead of in the ID stage in order to use the ALU
     * output to calculate the jump offset. However, this design incurs
     * a slight increase in CPI.
     */
    if (controller.isJumpInstr(instr)) {
        decodeInstr.setInput(Constants::BUBBLE);
        executeInstr.setInput(Constants::BUBBLE);

        /* We change the PC here for a very annoying reason.
         * Because of the linearity of the emulation, the PC will
         * select the proper jump address. However, it will have
         * to wait on a tick. This means the next output will be
         * the address of the 3rd instruction past the jump. Because
         * it has no instruction associated with it yet, we cannot just
         * bubble it. So, we cheat a little.
         */
        pc1.setInput(result);
#ifdef DEBUG
        std::cout << "Flushing pipeline for jump.\n";
        std::cout << "New PC addr.: " << result << std::endl;
#endif
    }

    // Check if branch was taken, if so: flush.
    if (controller.takeBranch(instr)) {
        decodeInstr.setInput(Constants::BUBBLE);
        executeInstr.setInput(Constants::BUBBLE);
        pc1.setInput(result);
#ifdef DEBUG
        std::cout << "Flushing pipeline for branch.\n";
        std::cout << "New PC addr.: " << result << std::endl;
#endif
    }
}


void riscv_emu::CPU::memory() {
    uint32_t instr = memInstr.getOutput();
    controller.setMemoryFlags(instr);
    //dataMem.setInput(controller.getMemMode(), controller.getMemRW(), aluOut.getOutput(), md2.getOutput());

    if(controller.getMemEnable()) {
        switch (controller.getMemRW()) {
            case READ:
                break;
            case WRITE:
                dataCache->write(controller.getMemMode(), aluOut.getOutput(), md2.getOutput());
                break;
        }
    }


    switch (controller.getWBSel()) {
        case ALU_SEL:
            wbData.setInput(aluOut.getOutput());
            break;
        case MEM_SEL:
            wbData.setInput(dataCache->read(controller.getMemMode(), aluOut.getOutput()));
            break;
        case PC_PLUS_4:
            wbData.setInput(pc4.getOutput() + 4);
            break;
    }

    wbInstr.setInput(instr);
    pc5.setInput(pc4.getOutput());
}

void riscv_emu::CPU::writeBack() {
    uint32_t instr = wbInstr.getOutput();
    controller.setWriteBackFlags(instr);
    uint32_t rd = (instr & Constants::RD_MASK) >> Constants::RD_SHIFT;

    registers.setWriteEnable(controller.getRegWEn());
    registers.setWriteData(wbData.getOutput());
    registers.setRd(rd);
}


void riscv_emu::CPU::run() {
    while(this->running) {
#ifdef DEBUG
        std::cout << "Clock: " << clock << std::endl;
        std::cout << "IF pc: " << pc1.getOutput() << std::endl;
        std::cout << "DC pc: " << pc2.getOutput() << "\tinstr: " << std::hex << decodeInstr.getOutput() << std::endl;
        std::cout << "EX pc: " << pc3.getOutput() << "\tinstr: " << std::hex << executeInstr.getOutput() << std::endl;
        std::cout << "ME pc: " << pc4.getOutput() << "\tinstr: " << std::hex << memInstr.getOutput() << std::endl;
        std::cout << "WB pc: " << pc5.getOutput() << "\tinstr: " << std::hex << wbInstr.getOutput() << std::endl;


#endif
        fetch();
        decode();
        execute();
        memory();
        writeBack();

        /*
         * Interlocking must be checked after all stages have been
         * processed. This is because interlocking requires information
         * from the MEM, EX, and WB stage. In a normal physical CPU
         * all stages execute simultaneously. However, in an emulator,
         * we only process each stage one after another (although
         * behavior will appear as if execution occurred simultaneously).
         * This same requirement applies for all control hazards that
         * require information from further stages.
         */
        interlock();

        this->clock++;
        tick();
    }

    // Must finish instructions left in the pipeline.
    for (int i = 0; i < 5; ++i) {
#ifdef DEBUG
        std::cout << "Clock: " << clock << std::endl;
        std::cout << "IF pc: " << pc1.getOutput() << std::endl;
        std::cout << "DC pc: " << pc2.getOutput() << "\tinstr: " << std::hex << decodeInstr.getOutput() << std::endl;
        std::cout << "EX pc: " << pc3.getOutput() << "\tinstr: " << std::hex << executeInstr.getOutput() << std::endl;
        std::cout << "ME pc: " << pc4.getOutput() << "\tinstr: " << std::hex << memInstr.getOutput() << std::endl;
        std::cout << "WB pc: " << pc5.getOutput() << "\tinstr: " << std::hex << wbInstr.getOutput() << std::endl;
#endif
        fetch();
        decode();
        execute();
        memory();
        writeBack();
        interlock();
        this->clock++;
        tick();
    }

#ifdef DEBUG
    std::cout << "\nRegister status:\n";
    for (int i = 0; i < 32; i++) {
        std::cout << "x" << i << ": " << registers.get(i) << std::endl;
    }
#endif
}


void riscv_emu::CPU::tick() {
    pc1.tick();
    pc2.tick();
    pc3.tick();
    pc4.tick();
    pc5.tick();

    registers.tick();

    decodeInstr.tick();
    executeInstr.tick();
    memInstr.tick();
    wbInstr.tick();

    rd1.tick();
    rd2.tick();
    md1.tick();
    md2.tick();
    md3.tick();
    aluOut.tick();
    wbData.tick();
}


void riscv_emu::CPU::interlock() {
    if (controller.doInterlock()) {
        // Insert bubbles in previous stages.
        executeInstr.setInput(Constants::BUBBLE);
        decodeInstr.setInput(decodeInstr.getOutput());

        // Write the current instruction PC back to PC.
        pc1.setInput(pc1.getOutput());
        pc3.setInput(pc2.getOutput());
        pc2.setInput(pc2.getOutput());
#ifdef DEBUG
        std::cout << "Inserting bubble...\n";
#endif
    }
}
