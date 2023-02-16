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
    this->memMode = MemMode::BYTE;
    this->pcSel = PCSel::PC_INCR;
    this->brUn = false;
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

void Controller::setFlags(uint32_t instr) {
    uint32_t opcode = instr & Constants::OPCODE_MASK;

    uint32_t func7;
    uint32_t func3;
    uint32_t imm_temp;

    switch (opcode) {
        case Constants::R_INSTR:
            this->aSel = 0;
            this->bSel = 0;
            this->regWEn = true;
            this->wbSel = WBSelect::ALU_SEL;
            this->invalidOp = false;
            this->pcSel = PCSel::PC_INCR;

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
            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;
            this->aSel = 0;
            this->bSel = true;
            this->memRw = MemRW::READ;
            this->wbSel = WBSelect::MEM_SEL;
            this->aluSel = ALU_Mode::ADD;
            this->regWEn = true;
            this->invalidOp = false;
            this->pcSel = PCSel::PC_INCR;

            switch (func3) {
                case 0b000: // LB
                    this->memMode = MemMode::BYTE;
                    break;
                case 0b001: // LH
                    this->memMode = MemMode::HALF_WORD;
                    break;
                case 0b010: // LW
                    this->memMode = MemMode::WORD;
                    break;
                case 0b100: // LBU
                    this->memMode = MemMode::BYTE_UPPER;
                    break;
                case 0b101: // LHU
                    this->memMode = MemMode::HALF_WORD_UPPER;
                    break;
                default:
                    this->invalidOp = true;
            }
            break;

        case Constants::I_INSTR:
            // Set control flags.
            this->aSel = 0;
            this->bSel = 1;
            this->regWEn = true;
            this->wbSel = WBSelect::ALU_SEL;
            this->invalidOp = false;
            this->pcSel = PCSel::PC_INCR;

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

        case Constants::S_INSTR:
            // Set control flags.
            this->aSel = 0;
            this->bSel = true;
            this->regWEn = false;
            this->memRw = MemRW::WRITE;
            this->aluSel = ALU_Mode::ADD;
            this->invalidOp = false;
            this->pcSel = PCSel::PC_INCR;

            func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;


            switch (func3) {
                case 0b000: // LB
                    this->memMode = MemMode::BYTE;
                    break;
                case 0b001: // LH
                    this->memMode = MemMode::HALF_WORD;
                    break;
                case 0b010: // LW
                    this->memMode = MemMode::WORD;
                    break;
                default:
                    this->memMode = MemMode::WORD;
                    this->invalidOp = true;
            }
            break;

        case Constants::B_INSTR:
            // Set control flags.
            this->aluSel = ALU_Mode::ADD;
            this->wbSel = WBSelect::PC_PLUS_4;
            this->invalidOp = false;
            this->regWEn = false;
            this->aSel = 1;
            this->bSel = 1;

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

        case Constants::U_INSTR:
            break;

        case Constants::J_INSTR:
            this->aluSel = ALU_Mode::ADD;
            this->wbSel = WBSelect::PC_PLUS_4;
            this->invalidOp = false;
            this->regWEn = true;
            this->aSel = 1;
            this->bSel = 1;
            this->pcSel = PCSel::ALU_PC_SEL;
            break;

        default:
            this->invalidOp = true;
            break;
    }
}

bool Controller::isInvalid() {
    return this->invalidOp;
}

MemMode Controller::getMemMode() {
    return this->memMode;
}

MemRW Controller::getMemRW() {
    return this->memRw;
}

WBSelect Controller::getWBSel() {
    return this->wbSel;
}

PCSel Controller::getPCSel() {
    return this->pcSel;
}

void Controller::setBranch(uint32_t instr, bool brEq, bool brLt) {
    uint32_t func3 = (instr & Constants::FUNC_3_MASK) >> Constants::FUNC_3_SHIFT;

    if ((instr & Constants::OPCODE_MASK) != Constants::B_INSTR) {
        return;
    }

    switch (func3) {
        case 0b000: // beq
            if (brEq) {
                this->pcSel = PCSel::ALU_PC_SEL;
                return;
            }
            break;

        case 0b001: // bne
            if(!brEq) {
                this->pcSel = PCSel::ALU_PC_SEL;
                return;
            }
            break;

        case 0b100: // blt or bltu
        case 0b110:
            if(brLt) {
                this->pcSel = PCSel::ALU_PC_SEL;
                return;
            }
            break;

        case 0b101: // bge or bgeu
        case 0b111:
            if(!brLt) {
                this->pcSel = PCSel::ALU_PC_SEL;
                return;
            }
            break;

        default:
            this->invalidOp = true;
            break;
    }

    this->pcSel = PCSel::PC_INCR;
}

bool Controller::getBrUn() {
    return this->brUn;
}

