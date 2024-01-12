#include <windows.h>
#include <stdio.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char windowTitle[256];
    char className[256];
    int length = GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
    GetClassName(hwnd, className, sizeof(className));

    if (length > 0) {
        printf("Window Title: %s, Class Name: %s\n", windowTitle, className);
    }

    return TRUE; 
}

int main() {
    EnumWindows(EnumWindowsProc, 0);
    return 0;
}

