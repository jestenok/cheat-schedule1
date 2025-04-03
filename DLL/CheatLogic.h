#include <windows.h>
#include <iostream>
#include <vector>
#include "Hook.h"


void CheatLogic() {
    void* addr = oGetPlayer("Local");
    printf("GetPlayer %p\n", addr);


    std::vector offsets = { 0x8, 0x10, 0x20 };
    uintptr_t aPlayerList = getWithOffsets(baseAssembly + 0x0377DA68, { 0xb8, 0x20, 0x0 });
    printf("PlayerList %p\n", aPlayerList);
}