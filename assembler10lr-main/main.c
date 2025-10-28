#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef void (*STORE_TEXT_FUNC)(const char*);
typedef void (*RETRIEVE_TEXT_FUNC)(char*);
typedef const char* (*GET_SHARED_MESSAGE_FUNC)(void);
typedef int (*GET_SHARED_CONNECTION_COUNT_FUNC)(void);
typedef void (*DISPLAY_SHARED_INFO_FUNC)(void);

void process_operations(HMODULE hDll, const char* process_name) {
    STORE_TEXT_FUNC StoreText = (STORE_TEXT_FUNC)GetProcAddress(hDll, "StoreText");
    RETRIEVE_TEXT_FUNC RetrieveText = (RETRIEVE_TEXT_FUNC)GetProcAddress(hDll, "RetrieveText");
    GET_SHARED_MESSAGE_FUNC GetSharedMessage = (GET_SHARED_MESSAGE_FUNC)GetProcAddress(hDll, "GetSharedMessage");
    GET_SHARED_CONNECTION_COUNT_FUNC GetSharedConnectionCount = (GET_SHARED_CONNECTION_COUNT_FUNC)GetProcAddress(hDll, "GetSharedConnectionCount");
    DISPLAY_SHARED_INFO_FUNC DisplaySharedInfo = (DISPLAY_SHARED_INFO_FUNC)GetProcAddress(hDll, "DisplaySharedInfo");
    
    if (StoreText && RetrieveText && GetSharedConnectionCount) {
        char buffer[256];
        char message[100];
    
        // Set our text
        sprintf(message, "Data from %s (PID: %lu)", process_name, GetCurrentProcessId());
        StoreText(message);
    
        // Short pause to allow other processes to see changes
        printf("Waiting 3 seconds...\n");
        fflush(stdout);
        Sleep(3000);
    
        // Read again (might be changed by another process)
        RetrieveText(buffer);
        printf("Text after waiting: '%s'\n", buffer);
        fflush(stdout);
    
        // Test with DLL message
        if (GetSharedMessage) {
            const char* dll_message = GetSharedMessage();
            printf("Message from DLL: %s\n", dll_message);
        
            // Store DLL message to shared memory
            StoreText(dll_message);
        
            // Read back to confirm
            RetrieveText(buffer);
            printf("Text after storing DLL message: '%s'\n", buffer);
            fflush(stdout);
        }
    
        printf("--- %s completed ---\n", process_name);
        fflush(stdout);
    
    } 
    else {
        printf("Error: not all functions found in DLL\n");
        if (!StoreText) printf(" - StoreText missing\n");
        if (!RetrieveText) printf(" - RetrieveText missing\n");
        if (!GetSharedConnectionCount) printf(" - GetSharedConnectionCount missing\n");
        fflush(stdout);
    }
}

int main(int argc, char* argv[]) {
    const char* process_name = (argc > 1) ? argv[1] : "Process 1";
    
    printf("Starting: %s\n", process_name);
    printf("Loading DLL...\n");
    fflush(stdout);
    
    HMODULE hDll = LoadLibrary("shared_mixed.dll");
    if (!hDll) {
        printf("Error loading DLL: %lu\n", GetLastError());
        printf("Press Enter to exit...\n");
        getchar();
        return 1;
    }
    
    printf("DLL loaded successfully!\n");
    fflush(stdout);
    
    // Perform operations
    process_operations(hDll, process_name);
    
    // Wait for input before unloading
    printf("\nPress Enter to unload DLL...\n");
    fflush(stdout);
    getchar();  // Wait for user input
    
    FreeLibrary(hDll);
    printf("DLL unloaded\n");
    
    printf("Press Enter to exit program...\n");
    getchar();
    
    return 0;
}