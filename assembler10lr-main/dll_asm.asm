section .shared read write shared
    global stored_text
    global connection_count

section .data
    default_msg db "Hello from ASM DLL!", 0

stored_text:
    times 256 db 0
connection_count:
    dd 0

section .text
bits 64

global DllMain
global SetText
global GetText
global GetDllMessage
global GetConnectionCount

DllMain:
    mov rcx, [rsp + 8]    ; hModule
    mov edx, [rsp + 16]   ; dwReason  
    mov r8, [rsp + 24]    ; lpReserved
    
    cmp edx, 1            ; DLL_PROCESS_ATTACH
    jne .not_attach
    
    ; Process attach - increment connection count
    lock inc dword [rel connection_count]
    
.not_attach:
    cmp edx, 0            ; DLL_PROCESS_DETACH
    jne .not_detach
    
    lock dec dword [rel connection_count]
    
.not_detach:
    mov eax, 1            ; Return TRUE
    ret

; SetText(const char* text)
SetText:
    mov rsi, rcx          ; source string
    lea rdi, [rel stored_text]
    mov rcx, 255
    cld
.copy_loop:
    lodsb
    test al, al
    jz .end_copy
    stosb
    loop .copy_loop
.end_copy:
    mov byte [rdi], 0
    ret

; GetText(char* buffer)  
GetText:
    mov rdi, rcx          ; destination buffer
    lea rsi, [rel stored_text]
    mov rcx, 256
    cld
    rep movsb
    ret

; const char* GetDllMessage()
GetDllMessage:
    lea rax, [rel default_msg]
    ret

; int GetConnectionCount()
GetConnectionCount:
    mov eax, [rel connection_count]
    ret