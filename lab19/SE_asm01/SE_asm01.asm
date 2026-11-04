.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.STACK 4096

.CONST 

Arr sdword 6,4,6,8,2,5,6,8,9,10

.DATA

str1 DB "Lab_19", 0
str2 DB "мин =  ", 0

MB_OK EQU 0  
HW DWORD ?

min sdword ?
max sdword ?

.CODE

main PROC
START:

push lengthof Arr
push offset Arr

call getMin

  mov EBX, EAX                               
  mov ECX, 10                                
  lea EDI, [str2 + 6] 
  ConvertToString:
  xor EDX, EDX                               
  div ECX                                    
  add DL, '0'                                
  mov [EDI], DL                              
  dec EDI                                    
  test EAX, EAX                              
  jnz ConvertToString
  

  INVOKE MessageBoxA, HW, offset str2,offset str1, 0

push 0
call ExitProcess
main ENDP

getMin PROC uses ecx esi ebx, parm01: dword, parm02: dword

      mov   ecx, parm02
      mov   esi, parm01
      mov   eax, [esi]

CYCLE: 

      mov   ebx, [esi]
      add   esi, 4
      cmp   eax, ebx
      jl    NOTMIN
      xchg  eax, ebx

NOTMIN: 

      loop  CYCLE

ret

getMin ENDP

END main
