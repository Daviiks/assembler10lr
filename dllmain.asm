section .data
    shared_data times 256 db 0
    default_msg db "Default DLL message", 0

section .text
bits 64

global DllMain
global SetText
global GetText
global GetMessage

; Точка входа DLL
DllMain:
    mov eax, 1
    ret

; Простая функция для тестирования
GetMessage:
    lea rax, [rel default_msg]  ; Правильно для x64!
    ret

; Функция установки текста
SetText:
    ; rcx = source string pointer
    mov rsi, rcx
    lea rdi, [rel shared_data]  ; Правильно для x64!
    mov rcx, 255
    cld
.copy_loop:
    lodsb
    test al, al
    jz .end_copy
    stosb
    dec rcx
    jnz .copy_loop
.end_copy:
    mov byte [rdi], 0
    ret

; Функция получения текста
GetText:
    ; rcx = destination buffer pointer
    mov rdi, rcx
    lea rsi, [rel shared_data]  ; Правильно для x64!
    mov rcx, 256
    cld
    rep movsb
    ret