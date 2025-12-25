.586
.model flat, stdcall

ExitProcess PROTO :DWORD
MessageBoxA PROTO :DWORD,:DWORD,:DWORD,:DWORD

includelib user32.lib
includelib kernel32.lib

.data
myBytes    BYTE    10h, 20h, 30h, 40h
myWords    WORD    8Ah, 38h, 44h, 5Fh, 99h
myDoubles  DWORD   1, 2, 3, 4, 5, 6
myPointer  DWORD   myDoubles

caption    BYTE    "Бурский Алексей Сергеевич", 0
message    BYTE    "Окно с кнопкой OK", 0

.code
main:
    mov ESI, OFFSET myWords
    mov AX, [ESI + 4]
    mov BX, [ESI + 2]

	push 0                  ; uType = MB_OK
    push OFFSET caption     ; заголовок
    push OFFSET message     ; текст
    push 0                  ; hWnd = NULL
    call MessageBoxA
    push 0
    
    call ExitProcess

end main