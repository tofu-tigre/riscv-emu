//
// Created by jq on 2/24/2023.
//

#ifndef RISCV_EMU_CACHE_H
#define RISCV_EMU_CACHE_H


#include <cstdint>
#include "../consts.h"

namespace riscv_emu {
    class Cache {
    private:
        virtual void evict(size_t index) = 0;
        virtual void fetch(uint32_t addr) = 0;
    public:

        virtual void flush() = 0;
        virtual uint32_t read(CacheByteSelect sel, uint32_t addr) = 0;
        virtual void write(CacheByteSelect sel, uint32_t addr, uint32_t val) = 0;
    };

}

#endif //RISCV_EMU_CACHE_H
