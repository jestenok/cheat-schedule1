#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <optional>
#include "Utils.hpp"
#include "Injector.hpp"
class InternalCheat {
public:
    char processName[64] = "Schedule I.exe";
    char moduleName[255];
    DWORD pid = 0;

    // Конструктор с параметром
    InternalCheat(const std::string& name) {
        strcpy(moduleName, name.c_str());
    }

    int loadDLL() {
        HMODULE hDll = LoadLibraryA(moduleName);
        if (!hDll) {
            std::cerr << "Failed to inject DLL. Error: " << GetLastError() << std::endl;
            return 1;
        }

        std::cout << "DLL injected" << std::endl;
        return 0;
    }

    int injectDLL() {
        ProcessWorker worker;
        pid = worker.findProcessId(processName);
        printf("Process ID: %d\n", pid);
        if (pid == 0) {
            std::cerr << "Process not found" << std::endl;
            return 1;
        }
        char DLLPath[MAX_PATH] = R"(D:\my_projects\cheat-schedule1\)";
        strcat(DLLPath, moduleName);
        Injector injector;
        return injector.injectByName(DLLPath, processName);
    }

    int detachDLL() {
        Injector injector;
        return injector.EjectDll(pid, moduleName);
    }
};
