#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

#define VK_CTRL 0x11
#define VK_C 0x43


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

void ClickAtCoords(HWND hWnd, int x, int y) {
    RECT rect;
    POINT pt;
    INPUT input = {0};
    GetClientRect(hWnd, &rect);
    pt.x = rect.left + x;
    pt.y = rect.top + y;
    ClientToScreen(hWnd, &pt);

    input.type = INPUT_MOUSE;
    input.mi.dx = pt.x * (65535 / GetSystemMetrics(SM_CXSCREEN)); 
    input.mi.dy = pt.y * (65535 / GetSystemMetrics(SM_CYSCREEN)); 
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

    SendInput(1, &input, sizeof(INPUT));
    printf("Send input at %d, %d\n", input.mi.dx, input.mi.dy);
}

void HandleCtrlC() {
    printf("ctrl c triggered\n");
    char* copied = obtainClipboard();
    printf("Copied text: %s\n", copied);
    HWND anki = FindWindowEx(NULL, NULL, "Qt652QWindowIcon", NULL);
    if (anki) {
        int x = 400;
        int y = 40;
        SetForegroundWindow(anki);
        ClickAtCoords(anki, x, y);
    } else {
        printf("anki window not found\n");
    }
}