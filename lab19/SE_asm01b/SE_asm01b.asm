.586P                                                               
.MODEL FLAT, STDCALL                                                
includelib ucrt.lib                                                 
includelib kernel32.lib                                             
includelib "..\Debug\SE_asm01a.lib"

ExitProcess         PROTO   : DWORD                                         
system              PROTO C : DWORD                                         
GetStdHandle        PROTO   : DWORD                                         
printConsole        PROTO : DWORD, : DWORD                                  
                                                                            
WriteConsoleA       PROTO   : DWORD, : DWORD, : DWORD, : DWORD, : DWORD     
SetConsoleTitleA    PROTO   : DWORD                                     

getmin      PROTO : DWORD, : DWORD
getmax      PROTO : DWORD, : DWORD

SetConsoleOutputCP  PROTO : DWORD                                   
                                                                    
SetConsoleCP PROTO : DWORD                                          


.STACK 4096                                                         

.CONST                                                              
endl        equ 0ah                                                 
str_endl    byte endl, 0                                            
str_pause   db 'pause', 0
zero        byte 40 dup(0)


.DATA                                                           
Array           SDWORD   -5, 5, 23, -1, 25, 8, -4, 22, 9, 0 
consoleTitle    BYTE    'Задание 3',0           
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


HW              =($ - text)                                         
consolehandle   DWORD 0h                                            

.CODE                                                               

int_to_char PROC uses eax ebx ecx edi esi,
                    pstr        : dword, 
                    intfield    : sdword 
    mov edi, pstr            
    mov eax, intfield        
    xor esi, esi             
    mov ebx, 10              

    test eax, eax              
    jge skip_neg               
    mov byte ptr [edi], '-'    
    inc edi                    
    neg eax                    
skip_neg:

convert_loop:
    xor edx, edx              
    div ebx                   
    push dx                   
    inc esi                   
    test eax, eax             
    jnz convert_loop          

write_loop:
    pop dx                   
    add dl, '0'              
    mov [edi], dl            
    inc edi                  
    dec esi                  
    jnz write_loop           

    ret                      
int_to_char ENDP


main PROC                                                           
START :                                                             
        INVOKE SetConsoleOutputCP, 1251d
        INVOKE SetConsoleCP, 1251d
        
        INVOKE getmin, OFFSET Array, LENGTHOF Array                         
        mov min, eax
        INVOKE int_to_char, OFFSET min_str, min
        INVOKE printConsole, OFFSET min_string, OFFSET  consoleTitle        
        INVOKE printConsole, OFFSET str_endl, OFFSET  consoleTitle
        
        INVOKE getmax, OFFSET Array, LENGTHOF Array                         
        mov max, eax
        INVOKE int_to_char, OFFSET max_str, max
        INVOKE printConsole, OFFSET max_string, OFFSET  consoleTitle        
        INVOKE printConsole, OFFSET str_endl, OFFSET  consoleTitle
        
        add eax, min                                                        
        mov result, eax
        INVOKE int_to_char, OFFSET string, result
        INVOKE printConsole, OFFSET text, OFFSET  consoleTitle      
        INVOKE printConsole, OFFSET str_endl, OFFSET  consoleTitle
        
        push offset str_pause                                       
        call system                                                 
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
