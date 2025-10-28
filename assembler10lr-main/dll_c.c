#include <windows.h>
#include <stdio.h>
#include <string.h>

// Declare ASM functions
extern void SetText(const char* text);
extern void GetText(char* buffer); 
extern const char* GetDllMessage(void);
extern int GetConnectionCount(void);

// C wrappers
__declspec(dllexport) void StoreText(const char* text) {
    printf("[C][PID: %lu] StoreText: '%s'\n", GetCurrentProcessId(), text);
    SetText(text);
}

__declspec(dllexport) void RetrieveText(char* buffer) {
    GetText(buffer);
    printf("[C][PID: %lu] RetrieveText: '%s'\n", GetCurrentProcessId(), buffer);
}

__declspec(dllexport) const char* GetSharedMessage() {
    return GetDllMessage();
}

__declspec(dllexport) int GetSharedConnectionCount() {
    int count = GetConnectionCount();
    printf("[C][PID: %lu] Connection count: %d\n", GetCurrentProcessId(), count);
    return count;
}

__declspec(dllexport) void DisplaySharedInfo() {
    char buffer[256];
    GetText(buffer);
    printf("[C][PID: %lu] Shared data - Text: '%s', Connections: %d\n", 
           GetCurrentProcessId(), buffer, GetConnectionCount());
}