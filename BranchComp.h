//
// Created by Jaeden Quintana on 2/15/23.
//

#ifndef RISCV_EMU_BRANCHCOMP_H
#define RISCV_EMU_BRANCHCOMP_H


#include <cstdint>

class BranchComp {
private:
    bool bUnsigned;
public:
    BranchComp();
    bool lessThan(uint32_t a, uint32_t b);
    bool equal(uint32_t a, uint32_t b);
    void setBUn(bool v);
};


#endif //RISCV_EMU_BRANCHCOMP_H
