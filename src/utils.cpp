#include "utils.h"

HMODULE hNtdll = NULL;
NtQueryInformationProcess_t _NtQueryInformationProcess = NULL;

namespace Utils {
    BOOL rpmb(HANDLE hProcess, PVOID pBaseAddress, PVOID pBuffer, SIZE_T size) {
        SIZE_T dwBytesRead;

        return ReadProcessMemory(hProcess, pBaseAddress, pBuffer, size, &dwBytesRead);
    }

    BOOL wpmb(HANDLE hProcess, PVOID pBaseAddress, PVOID pBuffer, SIZE_T size) {
        SIZE_T dwBytesWritten;

        return WriteProcessMemory(hProcess, pBaseAddress, pBuffer, size, &dwBytesWritten);
    }
    
    void InitLoadedLibraries() {
        hNtdll = GetModuleHandleA("ntdll.dll");
        if (!hNtdll) {
            printf("Failed to get handle to ntdll.dll.\n");
            return;
        }

        _NtQueryInformationProcess = (NtQueryInformationProcess_t)GetProcAddress(hNtdll, "NtQueryInformationProcess");
        if (!_NtQueryInformationProcess) {
            printf("Failed to get address of NtQueryInformationProcess.\n");
            return;
        }
    }

    DWORD getProcessPid(PCSTR processName) {
        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (Process32First(snapshot, &entry) == TRUE) {
            while (Process32Next(snapshot, &entry) == TRUE) {
                if (strcmp(entry.szExeFile, processName) == 0) {
                    CloseHandle(snapshot);
                    return entry.th32ProcessID;
                }
            }
        }

        CloseHandle(snapshot);
        
        return 0;
    }

    PBYTE readFileIntoBuffer(PCSTR fileName, PSIZE_T pSize) {
        FILE* fp;
        errno_t err;
        err = fopen_s(&fp, fileName, "rb");
        if (err)
            return NULL;

        fseek(fp, 0, SEEK_END);
        long fileSize = ftell(fp);
        rewind(fp);

        if (pSize)
            *pSize = fileSize;

        PBYTE buffer = new BYTE[fileSize];

        fread(buffer, fileSize, 1, fp);
        fclose(fp);

        return buffer;
    }

    PVOID findRemotePEB(HANDLE hProcess) {
        PROCESS_BASIC_INFORMATION pbi;
        NTSTATUS status = _NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
        if (!NT_SUCCESS(status))
            return 0;

        return pbi.PebBaseAddress;
    }
}