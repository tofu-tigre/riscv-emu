//
// Created by jq on 2/24/2023.
//

#ifndef RISCV_EMU_DIRECTMAPCACHE_H
#define RISCV_EMU_DIRECTMAPCACHE_H


#include "Cache.h"
#include "../consts.h"
#include "DRAM.h"

namespace riscv_emu {
    class DirectMapCache {
    private:
        static const size_t CACHE_LINE_SIZE = 64;
        static const size_t CACHE_SIZE = 32;
        int indexBitLength, tagBitLength, offsetBitLength; // Calculated at construction.

        uint32_t tag[CACHE_SIZE];
        uint8_t data[CACHE_SIZE][CACHE_LINE_SIZE];
        bool valid[CACHE_LINE_SIZE];
        bool dirty[CACHE_LINE_SIZE];
        DRAM* dram;

        void evict(size_t index);
        void fetch(uint32_t addr);
        size_t getIndex(uint32_t addr);
        size_t getTag(uint32_t addr);
        size_t getOffset(uint32_t addr);
        uint32_t getAddr(size_t index);
    public:
        explicit DirectMapCache(DRAM* aDram);
        uint32_t read(CacheByteSelect sel, uint32_t addr);
        void write(CacheByteSelect sel, uint32_t addr, uint32_t val);

        void flush();
    };
}


#endif //RISCV_EMU_DIRECTMAPCACHE_H
