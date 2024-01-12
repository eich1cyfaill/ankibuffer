#include "ankibuffer.h"

int main() {
    
    BOOL prevCtrlState = FALSE;
    BOOL prevCState = FALSE; 
    const char* targetProcessName = "anki.exe";
    DWORD processId = GetProcessIdByName(targetProcessName);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId); // getting access
    if (hProcess == NULL) {
        perror("Anki isn't launched");
        return 1;
    }

    char buffer[100];
    
        
    while (1) {
        SHORT ctrlState = GetAsyncKeyState(VK_CTRL);
        SHORT cState = GetAsyncKeyState(VK_C);
        BOOL isCtrlPressed = ctrlState & 0x8000;
        BOOL isCPressed = cState & 0x8000;
        if (isCPressed && !prevCState ) {
            if (isCtrlPressed) {
                HandleCtrlC();
            }
        }
        prevCState = isCPressed;
        prevCtrlState = isCtrlPressed;
        Sleep(200);
    }

    return 0;
}






