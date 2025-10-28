#include <windows.h>
#include <stdio.h>

typedef char* (*GET_MESSAGE_FUNC)();
typedef void (*SET_TEXT_FUNC)(const char*);
typedef void (*GET_TEXT_FUNC)(char*);

int main() {
    printf("Loading DLL...\n");
    
    HMODULE hDll = LoadLibrary("dllmain.dll");
    if (!hDll) {
        printf("Failed to load DLL. Error: %lu\n", GetLastError());
        return 1;
    }
    
    // Тестируем простую функцию
    GET_MESSAGE_FUNC GetMessage = (GET_MESSAGE_FUNC)GetProcAddress(hDll, "GetMessage");
    if (GetMessage) {
        printf("Simple message: %s\n", GetMessage());
    }
    
    // Тестируем SetText/GetText
    SET_TEXT_FUNC SetText = (SET_TEXT_FUNC)GetProcAddress(hDll, "SetText");
    GET_TEXT_FUNC GetText = (GET_TEXT_FUNC)GetProcAddress(hDll, "GetText");
    
    if (SetText && GetText) {
        printf("Testing SetText/GetText...\n");
        
        // Записываем текст
        SetText("This text was set from main program!");
        
        // Читаем текст
        char buffer[256];
        GetText(buffer);
        printf("Received from DLL: %s\n", buffer);
        
        // Еще один тест
        SetText("Another test message!");
        GetText(buffer);
        printf("Second test: %s\n", buffer);
    } else {
        printf("SetText or GetText not found\n");
    }
    
    FreeLibrary(hDll);
    printf("DLL unloaded\n");
    
    return 0;
}