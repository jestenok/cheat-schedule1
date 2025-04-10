#include "DLL/CheatLogic.h"

HMODULE ghModule;


void Detach() {
    hookManager.RemoveHook();
    log("Detached!\n\n");
}

int Main() {
    assembly = UnityResolve::Get(ASSEMBLY_CSHARP_NAME);
    if (!assembly) {
        printf("Assembly not found\n");
        return 1;
    }
    resolverHookManager.SetupHook();
    CheatLogic();
    return 0;
}

int Attach() {
    createConsole();
    log("Console created!");
    UnityResolve::Init(GAME_ASSEMBLY, UnityResolve::Mode::Il2Cpp);
    Main();
    CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
        FreeLibraryAndExitThread(ghModule, 0);
    }, nullptr, 0, nullptr);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    ghModule = hModule;
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