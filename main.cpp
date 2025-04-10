#include <windows.h>
#include <iostream>
#include <string>
#include "InternalCheat.hpp"


InternalCheat* pCheat = nullptr;


int main() {
    SetConsoleOutputCP(65001);
    std::string dllName = R"(CheatDLL.dll)";
    InternalCheat cheat(dllName);
    cheat.injectDLL();
    return 0;
}