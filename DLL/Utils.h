#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <vector>

#pragma region Utils
void createConsole() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void logMessageBox(LPCSTR str) {
    MessageBoxA(0, str, "Cheat Test", MB_OK);
}

template <typename... Args>
void log(const char str, Args... args) {
    printf(str, args...);
}
void log(const char* str) {
    printf("%s\n", str);
}
void log(const std::string& str) {
    printf("%s\n", str.c_str());
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

uintptr_t getWithOffsets(uintptr_t baseAddress, std::vector<uintptr_t> offsets) {
    uintptr_t address = baseAddress;
    uintptr_t pointer = *(uintptr_t *) address;
    for ( uintptr_t offset : offsets) {
        address = pointer + offset;
//        printf("address: %p\n", address);
        if (!isValidPointer((void *) address)) {  // Используем void* для проверки.
            log("Invalid address!");
            return 0;
        }
        pointer = *(uintptr_t *) address;
    }
    return address;
}

std::wstring ExtractUnicodeString(const char* str, size_t length) {
    const wchar_t* res = reinterpret_cast<const wchar_t *>((char*)str);
    return std::wstring(res, length); // безопасный вариант
}


#pragma endregion
