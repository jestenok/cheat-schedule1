#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <cstdlib>

#pragma region Utils
void createConsole() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void logMessageBox(LPCSTR str) {
    MessageBoxA(0, str, "Cheat Test", MB_OK);
}

void log(const char* str, ...) {
    printf("%s\n", str);
}

void uintToHexString(uintptr_t value, char *buffer) {
    const char *HexDigits = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[18] = '\0'; // Нулевой символ в конце строки

    for (int i = 17; i >= 2; i--) {
        buffer[i] = HexDigits[value & 0xF];
        value >>= 4;
    }
}

void logAddress(const char *label, uintptr_t address) {
    char buffer[19];
    uintToHexString(address, buffer);
    printf("%s: %s\n", label, buffer);
}

bool isValidPointer(void *ptr) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(ptr, &mbi, sizeof(mbi))) {
        return (mbi.State == MEM_COMMIT) && !(mbi.Protect & PAGE_NOACCESS);
    }
    return false;
}

uintptr_t getBaseAddress(const char* module) {
    auto baseAddress = (uintptr_t) GetModuleHandle(module);
    if (baseAddress == 0) {
        char logMessage[256] = "Failed to get base address for module: ";
        strcat(logMessage, module);
        log(logMessage);
    }
    return baseAddress;
}

uintptr_t getWithOffsets(uintptr_t baseAddress, const uintptr_t *offsets, size_t offsetCount) {
    uintptr_t address = baseAddress;
    uintptr_t pointer = *(uintptr_t *) address;
    for (size_t i = 0; i < offsetCount; i++) {
        address = pointer + offsets[i];
        if (!isValidPointer((void *) address)) {  // Используем void* для проверки.
            log("Invalid address!");
            return 0;
        }
        pointer = *(uintptr_t *) address;
    }
    return address;
}

uintptr_t findMethod(uintptr_t startAddress, uintptr_t endAddress, const unsigned char* pattern, size_t patternLength) {
    for (uintptr_t addr = startAddress; addr < endAddress - patternLength; addr++) {
        bool found = true;

        // Проходим по паттерну
        for (size_t i = 0; i < patternLength; i++) {
            // Если встречаем wildcard (0x00 для простоты), пропускаем байт
            if (pattern[i] == 0x00) {
                continue;
            }

            // Сравниваем текущий байт с паттерном
            if (*((unsigned char*)addr + i) != pattern[i]) {
                found = false;
                break;
            }
        }

        // Если совпадение найдено
        if (found) {
            return addr;
        }
    }

    return 0;
}

bool getSegmentRange(const char* moduleName, const char* segmentName, uintptr_t& start, uintptr_t& end) {
    printf("Module to find: %s\n", moduleName);
    HMODULE hModule = LoadLibraryA(moduleName);

    // Получаем заголовок PE-файла
    IMAGE_DOS_HEADER* dosHeader = (IMAGE_DOS_HEADER*)hModule;
    IMAGE_NT_HEADERS* ntHeaders = (IMAGE_NT_HEADERS*)((BYTE*)hModule + dosHeader->e_lfanew);
    // Ищем секцию .text
    IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(ntHeaders);
    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++, section++) {
        printf("Segment name: %s\n", section->Name);
        printf("Segment start: 0x%p\n", (uintptr_t)hModule + section->VirtualAddress);
        if (memcmp(section->Name, segmentName, 5) == 0) {
            start = (uintptr_t)hModule + section->VirtualAddress;
            end = start + section->Misc.VirtualSize;
//            return true;
        }
    }
    printf("Segment not found!");
    return false;
}

void PrintMemoryRegionInfo(void* addr) {
    MEMORY_BASIC_INFORMATION mbi;
    printf("Address to find: 0x%p\n", addr);
    if (VirtualQuery(addr, &mbi, sizeof(mbi)) == 0) {
        printf("VirtualQuery failed");
        return;
    }

    printf("Region name: %s\n", mbi.AllocationBase);
    printf("Region start: 0x%p\n", mbi.BaseAddress);
    printf("Region end: 0x%p\n", (void*)((uintptr_t)mbi.BaseAddress + mbi.RegionSize));
    printf("Region size: %zu\n", mbi.RegionSize);
    printf("State: %d\n", mbi.State);
    printf("Type: %d\n", mbi.Type);

    // Проверим, является ли эта память кодом (.text)
    if (mbi.Type == MEM_IMAGE) {
        printf("This is a mapped image section (DLL or EXE)\n");
    } else if (mbi.Type == MEM_PRIVATE) {
        printf("This is private memory\n");
    } else {
        printf("Unknown memory type\n");
    }

    // Определим, к какой секции памяти относится адрес
    if (mbi.BaseAddress <= addr && addr < (void*)((uintptr_t)mbi.BaseAddress + mbi.RegionSize)) {
        uintptr_t offset = (uintptr_t)addr - (uintptr_t)mbi.BaseAddress;
        printf("Offset: 0x%zx\n", offset);
    } else {
        printf("Address is not in this region\n");
    }
}

uintptr_t findPrivateRegion(uintptr_t& start, uintptr_t& end) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    uintptr_t current = (uintptr_t)si.lpMinimumApplicationAddress;
    while (current < (uintptr_t)si.lpMaximumApplicationAddress) {
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery((void*)current, &mbi, sizeof(mbi)) == 0) {
            printf("VirtualQuery failed");
            return 0;
        }

        if (mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE) {
            start = (uintptr_t)mbi.BaseAddress;
            end = start + mbi.RegionSize;
            printf("Private region start: 0x%p\n", start);
            printf("Private region end: 0x%p\n", end);
            return (uintptr_t)mbi.BaseAddress;
        }

        current += mbi.RegionSize;
    }

    return 0;
}

#pragma endregion

namespace Utils {
    void printError(const std::string& message, DWORD errorCode) {
        std::cerr << message << " Ошибка: " << errorCode << std::endl;
    }

    void loadEnvFile(const std::string& filePath) {
        std::ifstream envFile(filePath);
        if (!envFile.is_open()) {
            std::cerr << "Не удалось открыть .env файл: " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(envFile, line)) {
            // Пропускаем пустые строки и комментарии
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // Разделяем строку на ключ и значение
            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) {
                continue; // Пропускаем строки без '='
            }

            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);

            // Убираем пробелы вокруг ключа и значения
            key.erase(0, key.find_first_not_of(' '));
            key.erase(key.find_last_not_of(' ') + 1);
            value.erase(0, value.find_first_not_of(' '));
            value.erase(value.find_last_not_of(' ') + 1);

            // Устанавливаем переменную окружения
            if (!SetEnvironmentVariable(key.c_str(), value.c_str())) {
                std::cerr << "Не удалось установить переменную окружения: " << key << std::endl;
            }
        }
    }
}
#endif // UTILS_HPP