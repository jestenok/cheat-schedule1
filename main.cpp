#include <windows.h>
#include <iostream>
#include <string>
#include "InternalCheat.hpp"


InternalCheat* pCheat = nullptr;


int main() {
    SetConsoleOutputCP(65001);
    std::string dllName = R"(CheatDLL2.dll)";
    InternalCheat cheat(dllName);
    cheat.injectDLL();
    std::atexit([]() {
        if (pCheat) {
            pCheat->detachDLL();
        }
    });
    std::cin.get();
    cheat.detachDLL();
    return 0;
}