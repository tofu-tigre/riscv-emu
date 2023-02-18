//
// Created by jq on 2/17/2023.
//

#ifndef RISCV_EMU_REGISTERARRAY_H
#define RISCV_EMU_REGISTERARRAY_H

#include <cstdint>
#include "Tickable.h"

namespace riscv_emu {
    class RegisterArray : public Tickable {
    private:
        uint32_t registers[32];
        size_t rs1, rs2, rd;
        uint32_t dataWrite;
        bool writeEnable;
        bool error;

    public:
        RegisterArray();
        void tick() override;

        inline void setRs1(uint32_t val) { rs1 = val; }
        inline void setRs2(uint32_t val) { rs2 = val; }
        inline void setRd(uint32_t val) { rd = val; }
        inline void setWriteData(uint32_t data) { dataWrite = data; }
        inline void setWriteEnable(bool val) { writeEnable = val; }

        inline uint32_t getRs1Out() const { return registers[rs1]; }
        inline uint32_t getRs2Out() const { return registers[rs2];}
        inline uint32_t get(size_t i) const { return registers[i]; }
        inline void setInput(uint32_t rs1, uint32_t rs2, uint32_t rd, bool writeEn, uint32_t writeData) {
            this->rs1 = rs1;
            this->rs2 = rs2;
            this->rd = rd;
            this->writeEnable = writeEn;
            this->dataWrite = writeData;
        }
    };
}


#endif //RISCV_EMU_REGISTERARRAY_H
