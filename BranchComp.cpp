//
// Created by Jaeden Quintana on 2/15/23.
//

#include <cstring>
#include "BranchComp.h"

bool BranchComp::lessThan(uint32_t a, uint32_t b) {
    if (this->bUnsigned) {
        return a < b;
    }

    int32_t at;
    std::memcpy(&at, &a, sizeof(at));

    int32_t bt;
    std::memcpy(&bt, &b, sizeof(bt));
    return at < bt;
}

bool BranchComp::equal(uint32_t a, uint32_t b) {
    return a == b;

}

BranchComp::BranchComp() {
    this->bUnsigned = false;
}

void BranchComp::setBUn(bool v) {
    this->bUnsigned = v;
}
