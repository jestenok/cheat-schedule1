#ifndef THREAD_READER_HPP
#define THREAD_READER_HPP

#include <windows.h>
#include <tlhelp32.h>

class ThreadReader {
public:
    DWORD getMainThreadId(DWORD pid) const;
    bool readRegister(DWORD pid, CONTEXT& context) const;

private:
    static bool matchThreadOwner(const THREADENTRY32& te, DWORD pid);
};

#endif // THREAD_READER_HPP