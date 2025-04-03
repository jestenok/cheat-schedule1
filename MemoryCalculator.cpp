//
// Created by Jestenok on 22.03.2025.
//

#include "MemoryCalculator.h"
DWORD64 MemoryCalculator::getOffsetFromRbp(DWORD64 rbp) const {
    return rbp + 0x152;
}