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

typedef void (*MonoAssemblyFunc)(void* assembly, void* user_data);
std::vector<std::string> assemblies;
void OnAssemblyLoaded(void* assembly, void* user_data) {
    auto mono_assembly_get_image = (void*(*)(void*))GetProcAddress(GetModuleHandleA("mono-2.0-bdwgc.dll"), "mono_assembly_get_image");
    auto mono_image_get_name = (const char*(*)(void*))GetProcAddress(GetModuleHandleA("mono-2.0-bdwgc.dll"), "mono_image_get_name");

    if (mono_assembly_get_image && mono_image_get_name) {
        void* image = mono_assembly_get_image(assembly);
        const char* name = mono_image_get_name(image);
        if (name) {
            assemblies.push_back(name);
        }
    }
}
std::vector<std::string> GetAllAssemblies() {
    HMODULE monoModule = GetModuleHandleA("mono-2.0-bdwgc.dll");
    if (!monoModule) {
        printf("Mono module not loaded!\n");
        return {};
    }

    auto mono_assembly_foreach = (void(*)(MonoAssemblyFunc, void*))GetProcAddress(monoModule, "mono_assembly_foreach");
    if (!mono_assembly_foreach) {
        printf("mono_assembly_foreach not found!\n");
        return {};
    }

    mono_assembly_foreach(OnAssemblyLoaded, nullptr);
    return assemblies;
}


void* _AssemblyCSharp = nullptr;
void GetAssembly(void* assembly, void* user_data) {
    auto mono_assembly_get_image = (void*(*)(void*))GetProcAddress(GetModuleHandleA("mono-2.0-bdwgc.dll"), "mono_assembly_get_image");
    auto mono_image_get_name = (const char*(*)(void*))GetProcAddress(GetModuleHandleA("mono-2.0-bdwgc.dll"), "mono_image_get_name");

    if (mono_assembly_get_image && mono_image_get_name) {
        void* image = mono_assembly_get_image(assembly);
        const char* name = mono_image_get_name(image);
        if (name && strcmp(name, "Assembly-CSharp") == 0) {
            _AssemblyCSharp = assembly;
        }
    }
}
void* GetAssemblyCSharp() {
    HMODULE monoModule = GetModuleHandleA("mono-2.0-bdwgc.dll");
    if (!monoModule) {
        printf("Mono module not loaded!\n");
        return {};
    }

    auto mono_assembly_foreach = (void(*)(MonoAssemblyFunc, void*))GetProcAddress(monoModule, "mono_assembly_foreach");
    if (!mono_assembly_foreach) {
        printf("mono_assembly_foreach not found!\n");
        return {};
    }

    mono_assembly_foreach(GetAssembly, nullptr);
    return _AssemblyCSharp;
}


void CallFunc() {
//    auto f = reinterpret_cast<Jump_t>(jumpAddr);
//    f(&playerHealthInstance, 1000, true, 1);
}

#pragma endregion
