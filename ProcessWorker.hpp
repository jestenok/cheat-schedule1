//
// Created by Jestenok on 22.03.2025.
//

#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <optional>
#include <vector>
#include <algorithm>
#include "Utils.hpp"
class ProcessWorker {
public:
    const int bufferSize = 100;

    DWORD findProcessId(const std::string& targetName) const {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
        std::vector<std::pair<DWORD, SIZE_T>> candidates; // PID + Memory

        // Собираем все подходящие процессы
        if (Process32First(hSnapshot, &pe)) {
            do {
                if (matchProcessName(pe, targetName)) {
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
                    if (hProcess) {
                        PROCESS_MEMORY_COUNTERS pmc;
                        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                            candidates.emplace_back(pe.th32ProcessID, pmc.WorkingSetSize);
                        }
                        CloseHandle(hProcess);
                    }
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);

        // Выбираем процесс с максимальным потреблением памяти
        if (!candidates.empty()) {
            std::sort(candidates.begin(), candidates.end(),
                      [](const auto& a, const auto& b) { return a.second > b.second; });

            return candidates[0].first;
        }
        return 0;
    }

    DWORD64 getModuleBaseAddress(DWORD pid, const std::string &moduleName) {
        return (DWORD64) getModule(pid, moduleName).modBaseAddr;
    }

    tagMODULEENTRY32 getModule(DWORD pid, const std::string &moduleName) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            std::cerr << "Не удалось создать снимок процессов. Ошибка: " << GetLastError() << std::endl;
            return {0};
        }

        MODULEENTRY32 moduleEntry = {0};
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                if (_stricmp(moduleEntry.szModule, moduleName.c_str()) == 0) {
                    CloseHandle(hSnapshot);
                    return moduleEntry;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }

        CloseHandle(hSnapshot);
        return {0}; // Модуль не найден
    }

    void printAllModules(DWORD pid) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (hSnapshot == INVALID_HANDLE_VALUE) {
            std::cerr << "Не удалось создать снимок процессов. Ошибка: " << GetLastError() << std::endl;
            return;
        }

        MODULEENTRY32 moduleEntry = {0};
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &moduleEntry)) {
            do {
                std::cout << moduleEntry.szModule << std::endl;
            } while (Module32Next(hSnapshot, &moduleEntry));
        }

        CloseHandle(hSnapshot);
    }

    bool readMemory(DWORD pid, DWORD_PTR address, void* buffer, SIZE_T size) {
        HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (!process) {
            Utils::printError("Не удалось открыть процесс.", GetLastError());
            return false;
        }

        SIZE_T bytesRead;
        bool success = ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead);

        CloseHandle(process);
        return success && bytesRead == size;
    }

    char *getCharFromMemory(DWORD pid, DWORD address) {
        char *buffer = new char[bufferSize](); // Буфер для строки
        HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (process == nullptr) {
            Utils::printError("Не удалось открыть процесс.", GetLastError());
            return buffer;
        }
        SIZE_T bytesRead;
        if (ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), buffer, bufferSize, &bytesRead)) {
            // Убедимся, что строка заканчивается нулевым терминатором
            if (bytesRead < bufferSize) {
                buffer[bytesRead] = '\0'; // Добавляем нулевой терминатор
            } else {
                buffer[bufferSize - 1] = '\0'; // Обрезаем строку, если она слишком длинная
            }
        }
        CloseHandle(process);
        return buffer;
    }

    int getIntFromMemory(DWORD pid, DWORD address) {
        int buffer = 0;  // Читаем int напрямую без динамического выделения

        HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (process == nullptr) {
            Utils::printError("Не удалось открыть процесс.", GetLastError());
            return 0;
        }

        SIZE_T bytesRead;
        if (!ReadProcessMemory(process, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(int), &bytesRead) || bytesRead != sizeof(int)) {
            Utils::printError("Не удалось прочитать память.", GetLastError());
            CloseHandle(process);
            return 0;
        }

        CloseHandle(process);
        return buffer;
    }

    bool writeIntToMemory(DWORD pid, DWORD address, int value) {
        HANDLE process = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
        if (process == nullptr) {
            Utils::printError("Не удалось открыть процесс.", GetLastError());
            return false;
        }
        SIZE_T bytesWritten;
        if (!WriteProcessMemory(process, reinterpret_cast<LPVOID>(address), &value, sizeof(int), &bytesWritten)) {
            Utils::printError("Не удалось записать значение в память.", GetLastError());
        }
        CloseHandle(process);
        return true;

    }

private:
    bool matchProcessName(const PROCESSENTRY32 &pe, const std::string &processName) const {
        return processName == pe.szExeFile;
    }
};
