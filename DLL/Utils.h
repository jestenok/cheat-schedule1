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

uintptr_t FindPattern(uintptr_t pModuleBaseAddress, const char* szSignature, size_t nSelectResultIndex = NULL) {
    auto PatternToBytes = [](const char* szPattern) {
        auto m_iBytes = std::vector<int>{};
        const auto szStartAddr = const_cast<char*>(szPattern);
        const auto szEndAddr = const_cast<char*>(szPattern) + strlen(szPattern);

        for (auto szCurrentAddr = szStartAddr; szCurrentAddr < szEndAddr; ++szCurrentAddr) {
            if (*szCurrentAddr == '?') {
                ++szCurrentAddr;
                if (*szCurrentAddr == '?') ++szCurrentAddr;
                m_iBytes.push_back(-1);
            } else {
                m_iBytes.push_back(strtoul(szCurrentAddr, &szCurrentAddr, 16));
            }
        }

        return m_iBytes;
    };

    const auto pDosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
    const auto pNtHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + pDosHeader->e_lfanew);
    const auto dwSizeOfImage = pNtHeaders->OptionalHeader.SizeOfImage;
    auto m_iPatternBytes = PatternToBytes(szSignature);
    auto pScanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

    const auto m_iPatternSize = m_iPatternBytes.size();
    auto m_iPatternData = m_iPatternBytes.data();

    size_t nFoundResults = 0;

    for (size_t i = 0; i < dwSizeOfImage - m_iPatternSize; ++i) {
        bool bFound = true;
        for (size_t j = 0; j < m_iPatternSize; ++j) {
            if (m_iPatternData[j] != -1 && m_iPatternData[j] != pScanBytes[i + j]) {
                bFound = false;
                break;
            }
        }

        if (bFound) {
            if (nSelectResultIndex != 0) {
                if (nFoundResults < nSelectResultIndex) {
                    nFoundResults++;
                    continue;
                }
            }
            return reinterpret_cast<uintptr_t>(&pScanBytes[i]);
        }
    }

    return NULL;
}


#pragma endregion
