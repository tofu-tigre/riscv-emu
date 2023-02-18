//
// Created by jq on 2/14/2023.
//

#ifndef RISCV_EMU_DRAM_H
#define RISCV_EMU_DRAM_H


#include <cstdint>
#include "consts.h"
#include "Tickable.h"

namespace riscv_emu {
    class DRAM : public Tickable {
    private:
        uint8_t data[Constants::DRAM_SIZE];
        uint32_t addr;
        uint32_t writeData;
        uint32_t output;
        MemMode mode;
        MemRW rwSel;
        bool error;

        void read();
        void write();

    public:
        DRAM();
        DRAM(MemMode mode, MemRW rw, uint32_t addr, uint32_t write);
        void load(size_t addr, uint32_t* program, size_t size);
        uint32_t getOutput();
        void tick() override;

        inline void setAddr(size_t addr) { this->addr = addr; }
        inline void setWriteData(uint32_t data) { writeData = data; }
        inline void setMode(MemMode mode) { this->mode = mode; }
        inline void setRWSel(MemRW rw) { rwSel = rw; }

        inline void setInput(MemMode mode, MemRW rw, size_t addr, uint32_t writeData) {
            this->mode = mode;
            this->rwSel = rw;
            this->addr = addr;
            this->writeData = writeData;
        }


    };
}



#endif //RISCV_EMU_DRAM_H
