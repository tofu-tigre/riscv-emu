//
// Created by jq on 2/24/2023.
//

#include <algorithm>
#include <cmath>
#include "DirectMapCache.h"

riscv_emu::DirectMapCache::DirectMapCache(riscv_emu::DRAM *aDram) {
    std::fill_n(tag, CACHE_SIZE, 0);
    std::fill_n(dirty, CACHE_SIZE, false);
    std::fill_n(valid, CACHE_SIZE, false);
    for (auto & i : data) {
        std::fill_n(i, CACHE_LINE_SIZE, 0);
    }

    this->dram = aDram;
    this->offsetBitLength = (int) std::log2(CACHE_LINE_SIZE);
    this->indexBitLength = (int) std::log2(CACHE_SIZE);
    this->tagBitLength = 32 - offsetBitLength - indexBitLength;
}


uint32_t riscv_emu::DirectMapCache::read(CacheByteSelect sel, uint32_t addr) {
    // 1. Get the index and tag of the addr.
    size_t index = getIndex(addr);
    size_t addrTag = getTag(addr);
    size_t offset = getOffset(addr);

    // 2. Check if the cache line @ index is valid.
    if (!valid[index]) {
        fetch(addr);
    }

    // 3. Check if the tag of the cache line @ index matches.
    if (addrTag != this->tag[index]) {
        evict(index);
        fetch(addr);
    }

    // NOTE: It is undefined behavior if attempting to reading past cache line.
    // E.g. you read 4 bytes past the last offset.
    switch (sel) {
        case CACHE_SEL_BYTE:
            return data[index][offset];
        case CACHE_SEL_HALF_WORD:
            return data[index][offset] | (data[index][offset + 1] << 8);
        case CACHE_SEL_WORD:
            return data[index][offset] | (data[index][offset + 1] << 8) | (data[index][offset + 2] << 16) | (data[index][offset + 3] << 24);
        case CACHE_SEL_HALF_WORD_UPPER:
            return (data[index][offset + 2]) | (data[index][offset + 3] << 8);
        case CACHE_SEL_BYTE_UPPER:
            return (data[index][offset + 3]);
    }

    return 0;
}

void riscv_emu::DirectMapCache::evict(size_t index) {
    // We only evict valid and dirty lines.

    // Must writeback CACHE_LINE_SIZE bytes.
    uint32_t addr = getAddr(index);
    for (int i = 0; i < CACHE_LINE_SIZE; ++i) {
        data[index][i] = dram->readTemp(BYTE, addr + i);
    }

    dirty[index] = false;
    valid[index] = false;

}

void riscv_emu::DirectMapCache::flush() {
    for (int i = 0; i < CACHE_SIZE; ++i) {
        if (valid[i] && dirty[i]) {
            evict(i);
        }

        valid[i] = false;
    }
}

void riscv_emu::DirectMapCache::fetch(uint32_t addr) {
    // You only fetch invalid memory.
    // 1. Zero out the bottom two bits of ADDR.
    uint32_t naddr = addr & 0xfffffffc;

    uint32_t index = getIndex(addr);
    uint32_t addrtag = getTag(addr);
    this->tag[index] = addrtag;
    this->dirty[index] = false;
    this->valid[index] = true;

    // Must fetch CACHE_LINE_SIZE bytes.
    for (int i = 0; i < CACHE_LINE_SIZE; ++i) {
        data[index][i] = dram->readTemp(BYTE, naddr + i);
    }
}


size_t riscv_emu::DirectMapCache::getIndex(uint32_t addr) {
    addr = addr << tagBitLength;
    addr = addr >> (tagBitLength + offsetBitLength);
    return (size_t) addr;
}

size_t riscv_emu::DirectMapCache::getTag(uint32_t addr) {
    addr = addr >> (indexBitLength + offsetBitLength);
    return (size_t) addr;
}

size_t riscv_emu::DirectMapCache::getOffset(uint32_t addr) {
    addr = addr << (tagBitLength + indexBitLength);
    addr = addr >> (tagBitLength + indexBitLength);
    return (size_t) addr;
}

uint32_t riscv_emu::DirectMapCache::getAddr(size_t index) {
    uint32_t tg = this->tag[index];
    uint32_t ind = index;
    return (tg << (offsetBitLength + indexBitLength)) | (ind << offsetBitLength);
}

void riscv_emu::DirectMapCache::write(CacheByteSelect sel, uint32_t addr, uint32_t val) {
    // 1. Get the index and tag of the addr.
    size_t index = getIndex(addr);
    size_t addrTag = getTag(addr);
    size_t offset = getOffset(addr);

    // 2. Check if the cache line @ index is valid.
    if (!valid[index]) {
        fetch(addr);
    }

    // 3. Check if the tag of the cache line @ index matches.
    if (addrTag != this->tag[index]) {
        evict(index);
        fetch(addr);
    }

    // NOTE: It is undefined behavior if attempting to reading past cache line.
    // E.g. you read 4 bytes past the last offset.
    switch (sel) {
        case CACHE_SEL_BYTE:
            data[index][offset] = val;
            break;
        case CACHE_SEL_HALF_WORD:
            data[index][offset] = val & 0x000000FF;
            data[index][offset + 1] = (val & 0x0000FF00) >> 8;
            break;
        case CACHE_SEL_WORD:
            data[index][offset] = val & 0x000000FF;
            data[index][offset + 1] = (val & 0x0000FF00) >> 8;
            data[index][offset + 2] = (val & 0x00FF0000) >> 16;
            data[index][offset + 3] = (val & 0xFF000000) >> 24;
            break;
        case CACHE_SEL_HALF_WORD_UPPER:
        case CACHE_SEL_BYTE_UPPER:
            break;
    }
    dirty[index] = true;
}

