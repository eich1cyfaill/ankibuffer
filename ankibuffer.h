#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        perror("couldn't make a snapshot of proccesses\n");
        return 0;
    }

    if (!Process32First(hSnapshot, &pe32)) {
        perror("couldn't find a proccess\n");
        CloseHandle(hSnapshot);
        return 0;
    }

    do {
        if (strcmp(pe32.szExeFile, processName) == 0) {
            CloseHandle(hSnapshot);
            return pe32.th32ProcessID;
        }
    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return 0;
}


char* obtainClipboard() {
    char* clipboardText;
    if (!OpenClipboard(NULL)) {
        fprintf(stderr, "Error opening clipboard\n");
        return clipboardText;;
    }
    
    if (IsClipboardFormatAvailable(CF_TEXT)) {
        HANDLE hClipboardData = GetClipboardData(CF_TEXT);
        clipboardText = (char*)GlobalLock(hClipboardData);
        if (clipboardText != NULL) {
            printf("Copied text: %s\n", clipboardText);
            GlobalUnlock(hClipboardData);
        } else {
            fprintf(stderr, "Error getting clipboard data\n");
        }
    } else {
        printf("Clipboard does not contain text\n");
    }
    CloseClipboard();
    return clipboardText;
}

BOOL CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            char* copied = obtainClipboard();
            printf(copied);
            HWND anki = FindWindow(NULL, "Qt652QWindowIcon");
            printf("did");
            if (anki) {
                HWND addWindow = FindWindowEx(anki, NULL, "Qt652QWindowIcon", "Add");
                if (addWindow) {
                    PostMessage(addWindow, WM_COMMAND, BN_CLICKED, 0);
                    printf("Sent message to open the add window.\n");
                } else {
                    printf("Could not find the add window.\n");
                }
            } else {
                printf("anki window not found\n");
            }

            return TRUE;

        default:
            return FALSE;
    }
}