.586
.model flat, stdcall
option casemap:none

include windows.inc
includelib kernel32.lib
includelib user32.lib

GetStdHandle proto :dword
WriteConsoleA proto :dword, :dword, :dword, :dword, :dword
ExitProcess proto :dword
GetTickCount proto

.data
L1 BYTE 'демонстрации переполнения', 0
a_demonstrate_underflow DWORD 0
b_demonstrate_underflow DWORD 0
c_demonstrate_underflow DWORD 0
L2 DWORD 5
L3 BYTE 'a = ', 0
L4 DWORD 10
L5 BYTE 'b = ', 0
L6 BYTE 'c = ', 0
L7 DWORD 0
L8 DWORD 5
L9 BYTE 'Вызов библиотечных функций', 0
L10 BYTE 'pow(uint, uint)', 0
L11 DWORD 2
L12 DWORD 2
L13 DWORD 3
L14 DWORD 3
L15 BYTE 'random(uint min, uint max)', 0
L16 DWORD 0
L17 DWORD 100
L18 DWORD 1
L19 DWORD 9
L20 DWORD 0
result_bitwise_ops DWORD 0
op_type_bitwise_ops DWORD 0
L21 DWORD 0
L22 DWORD 15
L23 DWORD 1
L24 DWORD 240
L25 DWORD 2
L26 DWORD 2
L27 DWORD 200
L28 DWORD 95
L29 DWORD 55
test_uint_main DWORD 0
test_string_main BYTE 'S', 0
test_char_main BYTE '$', 0
L30 BYTE 'ДО', 0
L31 BYTE 'test_uint = ', 0
L32 BYTE 'test_string = ', 0
L33 BYTE 'test_char = ', 0
L34 DWORD 5
L35 BYTE 'ТЕСТОВАЯ СТРОКА', 0
L36 BYTE '64', 0
L37 BYTE 'ПОСЛЕ', 0
L38 BYTE 'test_uint = ', 0
L39 BYTE 'test_string = ', 0
L40 BYTE 'test_char = ', 0
L41 BYTE '=== ТЕСТ БИТОВЫХ ОПЕРАЦИЙ ===', 0
L42 BYTE 'bitwise_ops(0x12, 0x00) = ', 0
L43 DWORD 18
L44 DWORD 0
L45 BYTE 'bitwise_ops(0x12, 0x01) = ', 0
L46 DWORD 18
L47 DWORD 1
L48 BYTE 'bitwise_ops(0x12, 0x02) = ', 0
L49 DWORD 18
L50 DWORD 2
L51 BYTE 'bitwise_ops(0x12, 0x03) = ', 0
L52 DWORD 18
L53 DWORD 3
L54 DWORD 0
str_newline db 13, 10, 0
random_seed dd 0
console_handle dd 0
bytes_written dd 0
num_buffer db 32 dup(0)


.code
; Стандартные функции
pow proc a:dword, b:dword
    push ebp
    mov ebp, esp
    mov eax, 1
    mov ecx, [ebp+8]    ; a
    mov edx, [ebp+12]   ; b
    test edx, edx
    jz pow_done
pow_loop:
    imul eax, ecx
    dec edx
    jnz pow_loop
pow_done:
    pop ebp
    ret 8
pow endp

random proc min:dword, max:dword
    push ebp
    mov ebp, esp
    push ebx
    
    ; Инициализация seed если нужно
    cmp random_seed, 0
    jne seed_ok
    call GetTickCount
    mov random_seed, eax
seed_ok:
    ; Линейный конгруэнтный генератор
    mov eax, random_seed
    mov ecx, 1103515245
    mul ecx
    add eax, 12345
    mov random_seed, eax
    
    ; Получаем число в диапазоне
    shr eax, 16         ; берем средние биты
    and eax, 7FFFh
    
    mov ecx, [ebp+12]    ; максимум
    mov ebx, [ebp+8]     ; минимум
    sub ecx, ebx
    inc ecx
    xor edx, edx
    div ecx
    mov eax, ebx
    add eax, edx
    
    pop ebx
    pop ebp
    ret 8
random endp

; Вспомогательные функции
print_string proc strPtr:dword
    push ebp
    mov ebp, esp
    
    ; Вычисляем длину строки
    mov esi, [ebp+8]
    xor ecx, ecx
strlen_loop:
    cmp byte ptr [esi+ecx], 0
    je strlen_done
    inc ecx
    jmp strlen_loop
strlen_done:
    
    ; Выводим строку
    invoke WriteConsoleA, console_handle, esi, ecx, offset bytes_written, 0
    
    pop ebp
    ret 4
print_string endp

print_uint proc num:dword
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi
    
    ; Преобразуем число в строку
    mov eax, [ebp+8]
    lea edi, [num_buffer+31]
    mov byte ptr [edi], 0
    dec edi
    mov ebx, 10
    
convert_loop:
    xor edx, edx
    div ebx
    add dl, '0'
    mov [edi], dl
    dec edi
    test eax, eax
    jnz convert_loop
    
    ; Выводим строку
    lea eax, [edi+1]
    push eax
    call print_string
    
    ; Выводим перевод строки
    push offset str_newline
    call print_string
    
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret 4
print_uint endp



;=====================================

demonstrate_underflow proc

		;EXP
		;EXP
    push 5
    pop eax
		;EXP
		;EXP
		;EXP
    push 10
    pop eax
		;EXP
		;EXP
		;EXP
    push dword ptr [a]
    push dword ptr [b]
    pop ebx
    pop eax
    sub eax, ebx
    push eax
    pop eax
		;EXP
		;EXP
    push dword ptr [b]
    pop eax
		;EXP
    push 5
    pop eax
demonstrate_underflow endp


use_standard_library proc

		;EXP
		;EXP
		;EXP
    push 2
    pop eax
		;EXP
    push 3
    pop eax
		;EXP
		;EXP
    push 100
    pop eax
		;EXP
    push 9
    pop eax
		;EXP
use_standard_library endp


bitwise_ops proc

    mov eax, 
cmp eax, [L21]
je 1_case_1

cmp eax, [L23]
je 1_case_2

cmp eax, [L25]
je 1_case_3

jmp 1_default

1_case_1:
		;EXP
    push dword ptr [result]
    push 15
    pop ebx
    pop eax
    and eax, ebx
    push eax
    pop eax
jmp 1_end_switch:

1_case_2:
		;EXP
    push dword ptr [result]
    push 240
    pop ebx
    pop eax
    or eax, ebx
    push eax
    pop eax
jmp 1_end_switch:

1_case_3:
		;EXP
    push dword ptr [result]
    pop eax
    not eax
    push eax
    pop eax
jmp 1_end_switch:

1_default:
		;EXP
    push dword ptr [result]
    push 2
    pop ebx
    pop eax
    imul eax, ebx
    push eax
    pop eax
    mov eax, 
cmp eax, [L27]
je 2_case_1

jmp 2_default

2_case_1:
		;EXP
    push 95
    pop eax
jmp 2_end_switch:

2_default:
		;EXP
    push 55
    pop eax
2_end_switch:
jmp 1_end_switch:

1_end_switch:
		;EXP
bitwise_ops endp


main proc
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov console_handle, eax

		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
    push 5
    pop eax
		;EXP
    push offset L35
    pop eax
		;EXP
    push 64
    pop eax
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
		;EXP
    push 0
    pop eax
		;EXP
		;EXP
    push 1
    pop eax
		;EXP
		;EXP
    push 2
    pop eax
		;EXP
		;EXP
    push 3
    pop eax
		;EXP
	invoke ExitProcess, 0
main endp

