//
// Created by Jaeden Quintana on 2/15/23.
//

#ifndef RISCV_EMU_BRANCHCOMP_H
#define RISCV_EMU_BRANCHCOMP_H


#include <cstdint>
#include "consts.h"

namespace riscv_emu {
    class BranchComp {
    public:
        BranchComp();

        bool getComp(BranchCompType type, bool unsignedComp, uint32_t a, uint32_t b) const;
        bool lessThan(bool usgn, uint32_t a, uint32_t b) const;
        bool equal(bool usgn, uint32_t a, uint32_t b) const;
    };

}

#endif //RISCV_EMU_BRANCHCOMP_H
