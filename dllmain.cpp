//#include "DLL/CheatLogic.h"
#include "DLL/Utils.h"


int Attach() {
//    createConsole();
    printf("Console created!");

//    hookManager.SetupHook();
//    CheatLogic();
    printf("Done!");
    Sleep(1000 * 60 * 60);
    return 0;
}

void Detach() {
//    hookManager.RemoveHook();
    log("Detached!\n\n");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Attach, nullptr, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        Detach();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }
    return TRUE;
}