//
// Created by Jestenok on 22.03.2025.
//

#ifndef CHEAT_MEMORYCALCULATOR_H
#define CHEAT_MEMORYCALCULATOR_H

#include <windows.h>
#include <tlhelp32.h>


class MemoryCalculator {
public:
    DWORD64 getOffsetFromRbp(DWORD64 rbp) const;
};


#endif //CHEAT_MEMORYCALCULATOR_H
