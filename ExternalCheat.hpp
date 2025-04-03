#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <optional>
#include "Utils.hpp"
#include "Injector.hpp"
#include "ThreadReader.hpp"
class ExternalCheat {
public:
    int hack() {
        std::string processName = "REPO.exe"; // Имя процесса

        // Находим PID процесса
        ProcessWorker processWorker;

        DWORD pid = processWorker.findProcessId(processName);
        if (pid == 0) {
            Utils::printError("Процесс не найден.", GetLastError());
            return 1;
        }

        std::cout << "PID процесса '" << processName << "': " << pid << std::endl;

        ThreadReader reader;
        CONTEXT ctx;
        if (!reader.readRegister(pid, ctx)) {
            Utils::printError("Не удалось прочитать регистр RBP.", GetLastError());
            return 1;
        }

        uintptr_t moduleBase = processWorker.getModuleBaseAddress(pid, "mono-2.0-bdwgc.dll");
        std::cout << "[INFO] Module base: 0x" << std::hex << moduleBase << std::endl;

        uintptr_t staticOffset = 0x00774310;
        uintptr_t currentAddress = moduleBase + staticOffset;
        std::cout << "[INFO] Static address: 0x" << std::hex << currentAddress << std::endl;
        uintptr_t zeroValue = 0;
        processWorker.readMemory(pid, currentAddress, &zeroValue, sizeof(zeroValue));

        uintptr_t firstPointer = zeroValue + 0x390;
        std::cout << "[INFO] 1 Pointer: 0x" << std::hex << firstPointer << std::endl;
        uintptr_t firstValue = 0;
        processWorker.readMemory(pid, firstPointer, &firstValue, sizeof(firstValue));

        uintptr_t secondPointer = firstValue + 0x380;
        std::cout << "[INFO] 2 Pointer: 0x" << std::hex << secondPointer << std::endl;
        uintptr_t secondValue = 0;
        processWorker.readMemory(pid, secondPointer, &secondValue, sizeof(secondValue));

        uintptr_t healthAddress = secondValue + 0xAC;
        std::cout << "[INFO] Final Health Address: 0x" << std::hex << healthAddress << std::endl;
        int health = processWorker.getIntFromMemory(pid, healthAddress); // Исправлено на health
        std::cout << "Current health: " << std::dec << health << std::endl;

        // Запись нового значения
        int new_health = 2000;
        bool wrote = processWorker.writeIntToMemory(pid, healthAddress, new_health); // Исправлено на healthAddress
        std::cout << "Write status: " << wrote << std::endl;

        return 0;
    }
};
