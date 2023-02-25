//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_CONTROLLER_H
#define RISCV_EMU_CONTROLLER_H


#include <string>
#include "consts.h"

/**
 * Controls each stage of the RISC-V pipeline.
 * Contains only the control flags that affect the CPU.
 * The CPU is responsible for actually changing state given the
 * control flags.
 */
class Controller {

private:
    // Control flags
    bool regWEn;
    bool bSel;
    bool aSel;
    ALU_Mode aluSel;
    bool invalidOp;
    MemRW memRw;
    bool memEnable;
    WBSelect wbSel;
    CacheByteSelect memMode;
    PCSel pcSel;
    bool brUn;
    bool brEq;
    bool brLt;
    ImmType immSel;

    // Interlocking control flags
    bool re1, re2;
    uint32_t rs1, rs2, wsE, wsM, wsWB;
    bool weE, weM;

    // Jump kill controls
    bool didFlush;



public:
    Controller();

    /**
     * Sets the proper control flags for the decode stage.
     * @param instr - the instruction at the decode stage.
     */
    void setDecodeFlags(uint32_t instr);

    /**
     * Sets the proper control flags for the execute stage.
     * @param instr - the instruction at the execute stage.
     */
    void setExecuteFlags(uint32_t instr);

    /**
     * Sets the proper control flags for the memory stage.
     * @param instr - the instruction at the memory stage.
     */
    void setMemoryFlags(uint32_t instr);

    /**
     * Sets the proper control flags for the writeback stage.
     * @param instr - the instruction at the writeback stage.
     */
    void setWriteBackFlags(uint32_t instr);

    ALU_Mode getALUMode();
    CacheByteSelect getMemMode();
    ImmType getImmSel();
    WBSelect getWBSel();
    MemRW getMemRW();
    bool getRegWEn();
    bool getASel();
    bool getBSel();
    bool getBrUn();
    void setBranch(uint32_t instr, bool brEq, bool brLt);
    bool doInterlock();
    bool takeBranch(uint32_t instr);
    bool isJumpInstr(uint32_t instr);

    bool getMemEnable();
};


#endif //RISCV_EMU_CONTROLLER_H
