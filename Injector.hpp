//
// Created by Jestenok on 22.03.2025.
//
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <windows.h>
#include <psapi.h>
#include "ProcessWorker.hpp"
#include "Utils.hpp"

class Injector {
public:
    int injectByName(char* DLLPath, char* processName) const {
        // Находим PID процесса
        ProcessWorker worker;
        DWORD pid = worker.findProcessId(processName);
        return injectByPid(DLLPath, pid);
    }

    int injectByPid(char* DLLPath, DWORD pid) const {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        LPVOID AllocAddress = VirtualAllocEx(hProcess, nullptr, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(hProcess, AllocAddress, DLLPath, MAX_PATH, nullptr);
        HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, AllocAddress, 0, nullptr);

        return 0;
    }

    int EjectByName(char* processName, char* moduleName) const {
        ProcessWorker worker;
        DWORD pid = worker.findProcessId(processName);
        return EjectDll(pid, moduleName);
    }

    int EjectDll(DWORD pid, const char* dllName) const {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (!hProcess) return -1;

        // Находим адрес DLL в целевом процессе
        ProcessWorker worker;
        HMODULE hModule = worker.getModule(pid, dllName).hModule;
        if (!hModule) {
            CloseHandle(hProcess);
            return -2;
        }

        // Получаем адрес FreeLibrary
        LPVOID freeLibAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "FreeLibrary");
        if (!freeLibAddr) {
            CloseHandle(hProcess);
            return -3;
        }

        // Вызываем FreeLibrary удалённо
        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
                                            (LPTHREAD_START_ROUTINE)freeLibAddr, hModule, 0, NULL);
        if (!hThread) {
            CloseHandle(hProcess);
            return -4;
        }

        // Ждём завершения потока
        WaitForSingleObject(hThread, INFINITE);

        // Проверяем результат
        DWORD exitCode;
        GetExitCodeThread(hThread, &exitCode);

        CloseHandle(hThread);
        CloseHandle(hProcess);

        return (exitCode != 0) ? 0 : -5; // 0 = успех
    }
};