.586
.model flat, stdcall

includelib kernel32.lib
includelib user32.lib

ExitProcess PROTO, dwExitCode:DWORD
MessageBoxA PROTO, hWnd:DWORD, lpText:DWORD, lpCaption:DWORD, uType:DWORD
wsprintfA PROTO C, :DWORD, :DWORD, :VARARG

.data
    szCaption   db 'Результат сложения',0
    szFormat    db '5 + 3 = %d',0
    szBuffer    db 256 dup(0)
    
    num1        dd 5
    num2        dd 3
    result      dd ?

.code
start:
    mov eax, [num1]     ; EAX = 5
    add eax, [num2]     ; EAX = 5 + 3 = 8
    mov [result], eax   ; сохраняем результат
    
    invoke wsprintfA, offset szBuffer, offset szFormat, [result]
    invoke MessageBoxA, 0, offset szBuffer, offset szCaption, 0

    invoke ExitProcess, 0
end start