//
// Created by Jaeden Quintana on 2/15/23.
//

#include <cstring>
#include "BranchComp.h"

riscv_emu::BranchComp::BranchComp() {
}

bool riscv_emu::BranchComp::lessThan(bool usgn, uint32_t a, uint32_t b) const {
    if (usgn) {
        return a < b;
    }

    int32_t at;
    std::memcpy(&at, &a, sizeof(at));

    int32_t bt;
    std::memcpy(&bt, &b, sizeof(bt));
    return at < bt;
}

bool riscv_emu::BranchComp::equal(bool usgn, uint32_t a, uint32_t b) const {
    return a == b;
}

bool riscv_emu::BranchComp::getComp(BranchCompType type, bool unsignedComp, uint32_t a, uint32_t b) const {
    switch (type) {
        case EQUAL:
            return equal(unsignedComp, a, b);
        case LESS_THAN:
            return lessThan(unsignedComp, a, b);
    }
}
