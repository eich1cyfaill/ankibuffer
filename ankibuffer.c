#include "ankibuffer.h"

int main() {

    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        const char* targetProcessName = "anki.exe";
        DWORD processId = GetProcessIdByName(targetProcessName);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId); // launch application with access
        if (hProcess == NULL) {
            perror("Anki isn't launched");
            return 1;
        }
        char buffer[100];
        
        while (1) {
            // Your main application logic
        }
    } else {
        fprintf(stderr, "Error setting Ctrl+C handler\n");
        return 1;
    }

    

    // // LPVOID addressToRead =  Replace with the actual address - target process

    //
    // SIZE_T bytesRead;
    // if (ReadProcessMemory(hProcess, addressToRead, buffer, sizeof(buffer), &bytesRead)) {

    //     printf("Read %zu bytes from the target process.\n", bytesRead);
    // } else {
    //     perror("ReadProcessMemory");
    // }

    // CloseHandle(hProcess);

    return 0;
}






