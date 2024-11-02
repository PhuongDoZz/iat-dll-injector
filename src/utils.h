#ifndef __UTILS_H
#define __UTILS_H

#include <Windows.h>
#include <tlHelp32.h>
#include <stdio.h>
#include <Psapi.h>
#include <cstring>

#include "def.h"

typedef NTSTATUS(WINAPI* NtQueryInformationProcess_t)(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength
);

extern HMODULE hNtdll;
extern NtQueryInformationProcess_t _NtQueryInformationProcess;

namespace Utils {
    template <typename T>
    T rpm(HANDLE hProcess, PVOID pBaseAddress, SIZE_T size = sizeof(T)) {
        T buffer;
        SIZE_T dwBytesRead;
        ReadProcessMemory(hProcess, pBaseAddress, &buffer, size, &dwBytesRead);

        return buffer;
    }

    BOOL rpmb(HANDLE hProcess, PVOID pBaseAddress, PVOID pBuffer, SIZE_T size);

    template <typename T>
    BOOL wpm(HANDLE hProcess, PVOID pBaseAddress, T buffer, SIZE_T size = sizeof(T)) {
        SIZE_T dwBytesWritten;

        return WriteProcessMemory(hProcess, pBaseAddress, &buffer, size, &dwBytesWritten);
    }

    BOOL wpmb(HANDLE hProcess, PVOID pBaseAddress, PVOID pBuffer, SIZE_T size);


    VOID InitLoadedLibraries();
    DWORD getProcessPid(PCSTR processName);
    PBYTE readFileIntoBuffer(PCSTR fileName, PSIZE_T pSize);
    PVOID findRemotePEB(HANDLE hProcess);
}

#endif