#include <Windows.h>
#include <TlHelp32.h>
#include <cstdio>

#include "utils.h"
#include "mmap.h"

#define WINDOW_TITLE "[~] IAT DLL Injector V1.0"
#define REPO_LINK "https://github.com/rft0/iat-dll-injector"

int main(int argc, char* argv[]) {
    SetConsoleTitleA(WINDOW_TITLE);

    Utils::InitLoadedLibraries();

    char processName[256];
    char dllPath[256];

    printf("%s\n", WINDOW_TITLE);
    printf("[~] %s\n\n", REPO_LINK);

    if (argc >= 3) {
        strcpy_s(processName, argv[1]);
        strcpy_s(dllPath, argv[2]);
    } else {
        printf("[+] Enter the process name: ");
        scanf_s("%255s", processName, (unsigned)sizeof(processName));

        printf("[+] Enter the DLL path: ");
        scanf_s("%255s", dllPath, (unsigned)sizeof(dllPath));
    }

    DWORD pid = Utils::getProcessPid(processName);
    if (pid == 0) {
        printf("\n[-] Failed to get process ID.\n");
        printf("[-] Terminating...");
        Sleep(2000);
        return 1;
    }

    printf("\n[+] %s, PID: %lu\n", processName, pid);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL) {
        printf("\n[-] Failed to open process.\n");
        printf("[-] Terminating...");
        Sleep(2000);
        return 1;
    }

    SIZE_T fileSize = 0;
    PBYTE pDllSrc = Utils::readFileIntoBuffer(dllPath, &fileSize);
    if (pDllSrc == NULL) {
        printf("\n[-] Failed to read DLL.\n");
        printf("[-] Terminating...");
        Sleep(2000);
        return 1;
    }

    bool success = mmap(hProcess, pDllSrc, fileSize);
    if (!success) {
        delete[] pDllSrc;
        printf("\n[-] Failed to map DLL into process.\n");
        printf("[-] Terminating...");
        Sleep(2000);
        return 1;
    }

    printf("\n[+] DLL injected successfully.\n");
    printf("[+] Terminating...");
    Sleep(2000);

    delete[] pDllSrc;

    return 0;
}