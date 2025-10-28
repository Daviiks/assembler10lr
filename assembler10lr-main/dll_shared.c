#include <windows.h>
#include <stdio.h>
#include <string.h>

// Разделяемая секция между процессами
#pragma data_seg(".shared")
volatile LONG connection_count = 0;
char stored_text[256] = {0};
#pragma data_seg()

#pragma comment(linker, "/SECTION:.shared,RWS")

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            InterlockedIncrement(&connection_count);
            printf("[DLL] Process %lu connected. Total connections: %d\n", 
                   GetCurrentProcessId(), connection_count);
            break;
            
        case DLL_PROCESS_DETACH:
            InterlockedDecrement(&connection_count);
            printf("[DLL] Process %lu disconnected. Remaining connections: %d\n", 
                   GetCurrentProcessId(), connection_count);
            break;
    }
    return TRUE;
}

// Простые функции для работы с текстом
__declspec(dllexport) void StoreText(const char* text) {
    strncpy(stored_text, text, sizeof(stored_text)-1);
    stored_text[sizeof(stored_text)-1] = '\0';
}

__declspec(dllexport) void RetrieveText(char* buffer) {
    strncpy(buffer, stored_text, 255);
    buffer[255] = '\0';
}

__declspec(dllexport) const char* GetSharedMessage() {
    return "Hello from Shared DLL!";
}

__declspec(dllexport) int GetSharedConnectionCount() {
    return connection_count;
}

__declspec(dllexport) void DisplaySharedInfo() {
    printf("[DLL] Shared Info - Text: '%s', Connections: %d\n", 
           stored_text, connection_count);
}