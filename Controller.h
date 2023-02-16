//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_CONTROLLER_H
#define RISCV_EMU_CONTROLLER_H


#include "consts.h"


class Controller {

private:
    // Control flags:
    bool regWEn;
    bool bSel;
    bool aSel;
    ALU_Mode aluSel;
    bool invalidOp;
    MemRW memRw;
    WBSelect wbSel;
    MemMode memMode;
    PCSel pcSel;
    bool brUn;


public:
    Controller();
    void setFlags(uint32_t instr);
    ALU_Mode getALUMode();
    MemMode getMemMode();
    WBSelect getWBSel();
    MemRW getMemRW();
    PCSel getPCSel();
    bool getRegWEn();
    bool getASel();
    bool getBSel();
    bool isInvalid();
    bool getBrUn();
    void setBranch(uint32_t instr, bool brEq, bool brLt);

};


#endif //RISCV_EMU_CONTROLLER_H
