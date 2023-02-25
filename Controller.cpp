//
// Created by jq on 2/14/2023.
//

#include "Controller.h"

Controller::Controller() {
    this->bSel = 0;
    this->aSel = 0;
    this->aluSel = ALU_Mode::ADD;
    this->regWEn = 0;
    this->memRw = MemRW::READ;
    this->memMode = CacheByteSelect::CACHE_SEL_WORD;
    this->pcSel = PCSel::PC_INCR;
    this->brUn = false;
    memEnable = false;
    rs1 = rs2 = 0;
    re1 = re2 = 0;
    weE = wsE = 0;
    weM = wsM = 0;
    wsWB = 0;
    didFlush = false;
}


bool Controller::getASel() {
    return this->aSel;
}


ALU_Mode Controller::getALUMode() {
    return this->aluSel;
}


bool Controller::getRegWEn() {
    return this->regWEn;
}


bool Controller::getBSel() {
    return this->bSel;
}


CacheByteSelect Controller::getMemMode() {
    return this->memMode;
}


MemRW Controller::getMemRW() {
    return this->memRw;
}


WBSelect Controller::getWBSel() {
    return this->wbSel;
}


void Controller::setBranch(uint32_t instr, bool brEq, bool brLt) {
    this->brEq = brEq;
    this->brLt = brLt;
}


bool Controller::getBrUn() {
    return this->brUn;
}


ImmType Controller::getImmSel() {
    return this->immSel;
}


void Controller::setDecodeFlags(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;

    uint32_t func7;
    uint32_t func3;
    uint32_t imm_temp;

    rs1 = (instr & Constants::RS1_MASK) >> Constants::RS1_SHIFT;
    rs2 = (instr & Constants::RS2_MASK) >> Constants::RS2_SHIFT;

    switch (opcode) {
        case Constants::R_INSTR:
            this->aSel = 0;
            this->bSel = 0;
            this->re1 = this->re2 = true;
            break;

        case Constants::I_LOAD_INSTR:
            this->aSel = 0;
            this->immSel = IMM_I_TYPE;
            this->bSel = true;
            this->re1 = true;
            this->re2 = false;
            break;

        case Constants::I_INSTR:
            this->immSel = IMM_I_TYPE;
            this->aSel = 0;
            this->bSel = 1;
            this->regWEn = true;
            this->re1 = true;
            this->re2 = false;
            break;

        case Constants::I_JALR_INSTR:
            this->immSel = IMM_I_TYPE;
            this->aSel = 0;
            this->bSel = 1;
            this->re1 = true;
            this->re2 = false;
            break;

        case Constants::S_INSTR:
            this->immSel = IMM_S_TYPE;
            this->aSel = 0;
            this->bSel = true;
            this->re1 = true;
            this->re2 = true;

            break;

        case Constants::B_INSTR:
            // Set control flags
            this->immSel = IMM_B_TYPE;
            this->aSel = 1;
            this->bSel = 1;
            this->re1 = true;
            this->re2 = true;

            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;

            switch (func3) {
                case 0b000:
                case 0b001:
                case 0b100:
                    this->brUn = false;
                    break;

                case 0b101:
                case 0b110:
                case 0b111:
                    this->brUn = true;
                    break;
                default:
                    this->invalidOp = true;
                    break;
            }
            break;

        case Constants::U_LUI_INSTR:
            this->immSel = IMM_U_TYPE;
            this->bSel = true;
            this->immSel = IMM_U_TYPE;
            this->re1 = false;
            this->re2 = false;
            break;

        case Constants::U_AUIPC_INSTR:
            this->immSel = IMM_U_TYPE;
            this->aSel = true;
            this->bSel = true;
            this->immSel = IMM_U_TYPE;
            this->re1 = false;
            this->re2 = false;
            break;

        case Constants::J_INSTR:
            this->immSel = IMM_J_TYPE;
            this->aSel = 1;
            this->bSel = 1;
            this->re1 = false;
            this->re2 = false;
            break;

        default:
            this->invalidOp = true;
            break;
    }
}


void Controller::setExecuteFlags(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;
    wsE = (instr & Constants::RD_MASK) >> Constants::RD_SHIFT;

    uint32_t func7;
    uint32_t func3;
    uint32_t imm_temp;

    switch (opcode) {
        case Constants::R_INSTR:
            this->pcSel = PCSel::PC_INCR;
            weE = true;
            pcSel = PCSel::PC_INCR;

            func7 = (instr & Constants::FUNC_7_MASK) >> Constants::FUNC_7_SHIFT;
            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;

            switch (func3) {
                case 0b000: // add/sub
                    this->aluSel = (func7 == 0) ? ALU_Mode::ADD : ALU_Mode::SUB;
                    break;

                case 0b001: // shift left logical
                    this->aluSel = ALU_Mode::SLL;
                    break;

                case 0b010: // set on less than
                    this->aluSel = ALU_Mode::SLT;
                    break;

                case 0b011: // set on less than unsigned
                    this->aluSel = ALU_Mode::SLTU;
                    break;

                case 0b100: // xor
                    this->aluSel = ALU_Mode::XOR;
                    break;

                case 0b101: // shift right logical or shift right arithmetic
                    this->aluSel = (func7 == 0) ? ALU_Mode::SRL : ALU_Mode::SRA;
                    break;

                case 0b110: // or
                    this->aluSel = ALU_Mode::OR;
                    break;

                case 0b111: // and
                    this->aluSel = ALU_Mode::AND;
                    break;

                default:
                    this->invalidOp = true;
                    break;
            }

            break;

        case Constants::I_LOAD_INSTR:
            this->aluSel = ALU_Mode::ADD;
            this->pcSel = PCSel::PC_INCR;
            pcSel = PCSel::PC_INCR;
            weE = true;


            switch (func3) {
                case 0b000: // LB
                    this->memMode = CACHE_SEL_BYTE;
                    break;
                case 0b001: // LH
                    this->memMode = CACHE_SEL_HALF_WORD;
                    break;
                case 0b010: // LW
                    this->memMode = CACHE_SEL_WORD;
                    break;
                case 0b100: // LBU
                    this->memMode = CACHE_SEL_BYTE_UPPER;
                    break;
                case 0b101: // LHU
                    this->memMode = CACHE_SEL_HALF_WORD_UPPER;
                    break;
                default:
                    this->invalidOp = true;
            }
            break;

        case Constants::I_INSTR:
            // Set control flags.
            this->pcSel = PCSel::PC_INCR;
            weE = true;

            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;
            imm_temp = (((instr & Constants::I_IMM_MASK) >> Constants::I_IMM_SHIFT) >> 5);

            switch (func3) {
                case 0b000: // add imm.
                    this->aluSel = ALU_Mode::ADD;
                    break;

                case 0b001: // shift left logical imm.
                    // ensure that upper imm. bits are zero
                    if (imm_temp != 0) {
                        this->invalidOp = true;
                        break;
                    }
                    this->aluSel = ALU_Mode::SLL;
                    break;

                case 0b010: // set on less than imm.
                    this->aluSel = ALU_Mode::SLT;
                    break;

                case 0b011: // set on less than unsigned imm.
                    this->aluSel = ALU_Mode::SLTU;
                    break;

                case 0b100: // xor imm.
                    this->aluSel = ALU_Mode::XOR;
                    break;

                case 0b101: // shift right logical imm. or shift right arithmetic imm.
                    if (imm_temp == 0) {
                        this->aluSel = ALU_Mode::SRL;
                    } else if (imm_temp == 0b0100000) {
                        this->aluSel = ALU_Mode::SRA;
                    } else {
                        this->invalidOp = true;
                    }
                    break;

                case 0b110: // or imm.
                    this->aluSel = ALU_Mode::OR;
                    break;

                case 0b111: // and imm.
                    this->aluSel = ALU_Mode::AND;
                    break;

                default:
                    this->invalidOp = true;
                    break;
            }
            break;

        case Constants::I_JALR_INSTR:
            this->aluSel = ALU_Mode::ADD;
            this->pcSel = PCSel::ALU_PC_SEL;
            weE = true;
            break;

        case Constants::S_INSTR:
            this->aluSel = ALU_Mode::ADD;
            this->pcSel = PCSel::PC_INCR;
            weE = false;
            break;

        case Constants::B_INSTR:
            // Set control flags.
            this->aluSel = ALU_Mode::ADD;
            weE = false;
            pcSel = PCSel::PC_INCR;

            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;

            switch (func3) {
                case 0b000: // beq
                    if (brEq) {
                        this->pcSel = PCSel::ALU_PC_SEL;
                    }
                    break;

                case 0b001: // bne
                    if(!brEq) {
                        this->pcSel = PCSel::ALU_PC_SEL;
                    }
                    break;

                case 0b100: // blt or bltu
                case 0b110:
                    if(brLt) {
                        this->pcSel = PCSel::ALU_PC_SEL;
                    }
                    break;

                case 0b101: // bge or bgeu
                case 0b111:
                    if(!brLt) {
                        this->pcSel = PCSel::ALU_PC_SEL;
                    }
                    break;

                default:
                    this->invalidOp = true;
                    break;
            }
            break;

        case Constants::U_LUI_INSTR:
            this->pcSel = PC_INCR;
            this->aluSel = B;
            weE = true;
            break;

        case Constants::U_AUIPC_INSTR:
            this->pcSel = PC_INCR;
            this->aluSel = ADD;
            weE = true;
            break;

        case Constants::J_INSTR:
            this->pcSel = PCSel::ALU_PC_SEL;
            this->aluSel = ALU_Mode::ADD;
            weE = true;
            break;

        default:
            this->invalidOp = true;
            break;
    }
}


void Controller::setMemoryFlags(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;
    wsM = (instr & Constants::RD_MASK) >> Constants::RD_SHIFT;
    uint32_t func3;

    switch (opcode) {
        case Constants::R_INSTR:
            memEnable = false;
            this->memRw = MemRW::READ;
            this->wbSel = WBSelect::ALU_SEL;
            weM = true;
            break;

        case Constants::I_LOAD_INSTR:
            memEnable = true;
            this->wbSel = WBSelect::MEM_SEL;
            weM = true;
            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;
            this->memRw = MemRW::READ;

            switch (func3) {
                case 0b000: // LB
                    this->memMode = CACHE_SEL_BYTE;
                    break;
                case 0b001: // LH
                    this->memMode = CACHE_SEL_HALF_WORD;
                    break;
                case 0b010: // LW
                    this->memMode = CACHE_SEL_WORD;
                    break;
                case 0b100: // LBU
                    this->memMode = CACHE_SEL_BYTE_UPPER;
                    break;
                case 0b101: // LHU
                    this->memMode = CACHE_SEL_HALF_WORD_UPPER;
                    break;
                default:
                    this->invalidOp = true;
            }
            break;

        case Constants::I_INSTR:
            // Set control flags.
            weM = true;
            this->wbSel = WBSelect::ALU_SEL;
            this->memRw = MemRW::READ;
            memEnable = false;

            break;

        case Constants::I_JALR_INSTR:
            this->wbSel = WBSelect::PC_PLUS_4;
            this->memRw = MemRW::READ;
            memEnable = false;
            weM = true;
            break;

        case Constants::S_INSTR:
            this->memRw = MemRW::WRITE;
            memEnable = true;
            weM = false;

            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;
            switch (func3) {
                case 0b000: // LB
                    this->memMode = CACHE_SEL_BYTE;
                    break;
                case 0b001: // LH
                    this->memMode = CACHE_SEL_HALF_WORD;
                    break;
                case 0b010: // LW
                    this->memMode = CACHE_SEL_WORD;
                    break;
                case 0b100: // LBU
                    this->memMode = CACHE_SEL_BYTE_UPPER;
                    break;
                case 0b101: // LHU
                    this->memMode = CACHE_SEL_HALF_WORD_UPPER;
                    break;
                default:
                    this->invalidOp = true;
            }
            break;

        case Constants::B_INSTR:
            // Set control flags.
            this->memRw = MemRW::READ;
            memEnable = false;
            weM = false;
            break;

        case Constants::U_LUI_INSTR:
            this->memRw = READ;
            memEnable = false;
            this->wbSel = ALU_SEL;
            weM = true;
            break;

        case Constants::U_AUIPC_INSTR:
            weM = true;
            this->memRw = READ;
            this->wbSel = ALU_SEL;
            memEnable = false;
            break;

        case Constants::J_INSTR:
            weM = true;
            this->memRw = MemRW::READ;
            memEnable = false;
            this->wbSel = WBSelect::PC_PLUS_4;
            break;

        default:
            this->invalidOp = true;
            break;
    }

}


void Controller::setWriteBackFlags(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;
    wsWB = (instr & Constants::RD_MASK) >> Constants::RD_SHIFT;

    switch (opcode) {
        case Constants::R_INSTR:
            this->regWEn = true;
            break;

        case Constants::I_LOAD_INSTR:

            this->regWEn = true;
            break;

        case Constants::I_INSTR:
            // Set control flags.
            this->regWEn = true;


            break;

        case Constants::I_JALR_INSTR:
            this->regWEn = true;
            break;

        case Constants::S_INSTR:
            this->regWEn = false;
            this->memRw = MemRW::WRITE;

            break;

        case Constants::B_INSTR:
            // Set control flags.

            this->regWEn = false;
            break;

        case Constants::U_LUI_INSTR:

            this->regWEn = true;

            break;

        case Constants::U_AUIPC_INSTR:

            this->regWEn = true;
            this->wbSel = ALU_SEL;
            break;

        case Constants::J_INSTR:

            this->regWEn = true;

            break;

        default:
            this->invalidOp = true;
            break;
    }

}


bool Controller::doInterlock() {


    if(!re1 && !re2) { // If we're not reading either register, do nothing.
        if (didFlush) {
            didFlush = false;
        }
        return false;
    }


    if (re1 && (rs1 != 0) && !didFlush) {
        // Check if match in execute stage only for r1.
        if(((rs1 == wsE) && weE) || ((rs1 == wsM) && weM) || ((rs1 == wsWB) && regWEn)) {
            return true;
        }
    }

    if(re2 && !didFlush && (rs2 != 0) && (((rs2 == wsE) && weE) || ((rs2 == wsM) && weM) || ((rs2 == wsWB) && regWEn))) {
        return true;
    }

    if (didFlush) {
        didFlush = false;
    }

    return false;
}


bool Controller::takeBranch(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;
    if (opcode != Constants::B_INSTR) {
        return false;
    }

    uint32_t func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;

    switch (func3) {
        case 0b000: // beq
            if (brEq) {
                didFlush = true;
                return true;
            }
            break;

        case 0b001: // bne
            if(!brEq) {
                didFlush = true;
                return true;
            }
            break;

        case 0b100: // blt or bltu
        case 0b110:
            if(brLt) {
                didFlush = true;
                return true;
            }
            break;

        case 0b101: // bge or bgeu
        case 0b111:
            if(!brLt) {
                didFlush = true;
                return true;
            }
            break;

        default:
            break;
    }
    return false;
}


bool Controller::isJumpInstr(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;
    if (opcode == Constants::J_INSTR || opcode == Constants::I_JALR_INSTR) {
        didFlush = true;
        return true;
    }
    return false;
}

bool Controller::getMemEnable() {
    return memEnable;
}

