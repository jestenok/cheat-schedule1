#include "ThreadReader.hpp"
#include <windows.h>
#include <iostream>

DWORD ThreadReader::getMainThreadId(DWORD pid) const {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Не удалось создать снимок потоков. Ошибка: " << GetLastError() << std::endl;
        return 0;
    }

    THREADENTRY32 te;
    te.dwSize = sizeof(THREADENTRY32);

    if (!Thread32First(hSnapshot, &te)) {
        std::cerr << "Не удалось получить первый поток. Ошибка: " << GetLastError() << std::endl;
        CloseHandle(hSnapshot);
        return 0;
    }

    DWORD mainThreadId = 0;
    do {
        if (matchThreadOwner(te, pid)) {
            mainThreadId = te.th32ThreadID;
            break;
        }
    } while (Thread32Next(hSnapshot, &te));

    CloseHandle(hSnapshot);
    return mainThreadId;
}

bool ThreadReader::readRegister(DWORD pid, CONTEXT& context) const {
    DWORD mainThreadId = getMainThreadId(pid);
    if (mainThreadId == 0) {
        return false;
    }

    HANDLE hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, mainThreadId);
    if (hThread == nullptr) {
        std::cerr << "Не удалось открыть поток. Ошибка: " << GetLastError() << std::endl;
        return false;
    }

    CONTEXT ctx;
    ctx.ContextFlags = CONTEXT_FULL;
    if (!GetThreadContext(hThread, &ctx)) {
        std::cerr << "Не удалось получить контекст потока. Ошибка: " << GetLastError() << std::endl;
        CloseHandle(hThread);
        return false;
    }

    context = ctx;
    CloseHandle(hThread);
    return true;
}

bool ThreadReader::matchThreadOwner(const THREADENTRY32& te, DWORD pid) {
    return te.th32OwnerProcessID == pid;
}