.586P                                                               
.MODEL FLAT, STDCALL                                                
;includelib ucrt.lib                                                
includelib libucrt.lib
includelib kernel32.lib                                             
includelib "..\Debug\SE_asm01d.lib"

ExitProcess         PROTO   : DWORD                                         
system              PROTO C : DWORD                                         
GetStdHandle        PROTO   : DWORD                                         
printConsole        PROTO : DWORD, : DWORD                                  
                                                                            
WriteConsoleA       PROTO   : DWORD, : DWORD, : DWORD, : DWORD, : DWORD     
SetConsoleTitleA    PROTO   : DWORD                                     

EXTRN getmin : proc
EXTRN getmax : proc
EXTRN get_pause : proc

SetConsoleOutputCP  PROTO : DWORD                                   
                                                                    
SetConsoleCP PROTO : DWORD                                          


.STACK 4096                                                         

.CONST                                                              
endl        equ 0ah                                                 
str_endl    byte endl, 0                                            
str_pause   db 'pause', 0

.DATA                                                               
Array           SDWORD   -5, 5, 23, -1, 25, 9, -4, 22, 9, 0 
consoleTitle    BYTE    'Задание 6',0           
text            BYTE     "Задача : getmax + getmin = "
string          BYTE     40 dup(0)
min_string      BYTE    "Минимальное значение = "
min_str         BYTE     40 dup(0)
max_string      BYTE    "Максимальное значение = "
max_str         BYTE     40 dup(0)

messageSize     DWORD   ?
min             SDWORD  ?
max             SDWORD  ?
result          SDWORD  ?

.CODE                                                               

int_to_char PROC uses eax ebx ecx edi esi,
                pstr        : dword, 
                intfield    : sdword 

    mov edi, pstr
    mov esi, 0
    mov eax, intfield                                               
    cdq
    test eax, 80000000h                                             
    jz convert_positive
    neg eax
    mov byte ptr [edi], '-' 
    inc edi

convert_positive:
    mov ebx, 10
    xor ecx, ecx  
    test eax, eax
    jz store_zero

convert_loop:
    xor edx, edx
    div ebx
    push edx
    inc ecx
    test eax, eax
    jnz convert_loop

store_digits:
    pop edx
    add dl, '0'
    mov [edi], dl
    inc edi
    loop store_digits

    mov byte ptr [edi], 0  
    ret

store_zero:
    mov byte ptr [edi], '0'
    inc edi
    mov byte ptr [edi], 0  
    ret
    
int_to_char ENDP


main PROC                                                           
START :                                                             
        INVOKE SetConsoleOutputCP, 1251d
        INVOKE SetConsoleCP, 1251d
        push LENGTHOF Array                                         
        push OFFSET Array
        call getmin
        mov min, eax
        INVOKE int_to_char, OFFSET min_str, min
        INVOKE printConsole, OFFSET min_string, OFFSET  consoleTitle        
        INVOKE printConsole, OFFSET str_endl, OFFSET  consoleTitle
        push LENGTHOF Array                                         
        push OFFSET Array
        call getmax                     
        mov max, eax
        INVOKE int_to_char, OFFSET max_str, max
        INVOKE printConsole, OFFSET max_string, OFFSET  consoleTitle        
        INVOKE printConsole, OFFSET str_endl, OFFSET  consoleTitle
        add eax, min                                                        
        mov result, eax
        INVOKE int_to_char, OFFSET string, result
        INVOKE printConsole, OFFSET text, OFFSET  consoleTitle      
        INVOKE printConsole, OFFSET str_endl, OFFSET  consoleTitle
                                                                    
        call get_pause                                              
        push - 1                                                    
        call ExitProcess                                            


main ENDP                                                           
printConsole     PROC uses eax ebx ecx edi esi,
                        pstr    : dword,
                        ptitle  : dword

    INVOKE SetConsoleTitleA, ptitle
    INVOKE GetStdHandle, -11
    mov esi, pstr                                                   
    mov edi, -1                                                     
count:                                                              
    inc edi                                                         
    cmp byte ptr [esi+edi], 0
    jne count                                                       

    INVOKE WriteConsoleA, eax, pstr, edi, 0, 0                      

    ret
printConsole ENDP

end main                                                            
