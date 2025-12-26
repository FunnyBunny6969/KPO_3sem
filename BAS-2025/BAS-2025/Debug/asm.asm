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
L8 BYTE 'Вызов библиотечных функций', 0
L9 BYTE 'pow(uint, uint)', 0
L10 DWORD 2
L11 DWORD 2
L12 DWORD 3
L13 DWORD 3
L14 BYTE 'random(uint min, uint max)', 0
L15 DWORD 0
L16 DWORD 100
L17 DWORD 1
L18 DWORD 9
L19 DWORD 0
result_bitwise_ops DWORD 0
op_type_bitwise_ops DWORD 0
L20 DWORD 0
L21 DWORD 15
L22 DWORD 1
L23 DWORD 240
L24 DWORD 2
L25 DWORD 2
bukva_bitwise_ops BYTE '$', 0
L26 BYTE '70', 0
L27 BYTE '65', 0
L28 BYTE '65', 0
L29 BYTE '70', 0
L30 BYTE '70', 0
word_bitwise_ops BYTE 'S', 0
L31 BYTE 'WORD', 0
L32 BYTE 'hhh', 0
L33 BYTE 'hhh', 0
L34 BYTE 'WORD', 0
L35 BYTE 'word', 0
test_uint_main DWORD 0
test_string_main BYTE 'S', 0
test_char_main BYTE '$', 0
L36 BYTE 'ДО', 0
L37 BYTE 'test_uint = ', 0
L38 BYTE 'test_string = ', 0
L39 BYTE 'test_char = ', 0
L40 DWORD 5
L41 BYTE 'ТЕСТОВАЯ СТРОКА', 0
L42 BYTE '64', 0
L43 BYTE 'ПОСЛЕ', 0
L44 BYTE 'test_uint = ', 0
L45 BYTE 'test_string = ', 0
L46 BYTE 'test_char = ', 0
L47 BYTE '=== ТЕСТ БИТОВЫХ ОПЕРАЦИЙ ===', 0
L48 BYTE 'bitwise_ops(0x12, 0x00) = ', 0
L49 DWORD 18
L50 DWORD 0
L51 BYTE 'bitwise_ops(0x12, 0x01) = ', 0
L52 DWORD 18
L53 DWORD 1
L54 BYTE 'bitwise_ops(0x12, 0x02) = ', 0
L55 DWORD 18
L56 DWORD 2
L57 BYTE 'bitwise_ops(0x12, 0x03) = ', 0
L58 DWORD 18
L59 DWORD 3
L60 DWORD 0
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

demonstrate_underflow proc
    push ebp
    mov ebp, esp
    sub esp, 12
    ; print
    push 
    call print_uint
    add esp, 4
    ; Объявление переменной a
    ; Объявление переменной b
    ; Объявление переменной c
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    mov eax, 
    mov esp, ebp
    pop ebp
    ret
end_switch:
use_standard_library proc
    push ebp
    mov ebp, esp
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    mov eax, 
    mov esp, ebp
    pop ebp
    ret
end_switch:
bitwise_ops proc result, op_type
    push ebp
    mov ebp, esp
    ; switch
    mov eax, 
    ; switch body
case_0:
	jmp end_switch
case_1:
	jmp end_switch
case_2:
	jmp end_switch
default_case:
	jmp end_switch
end_switch:
    ; Объявление переменной bukva
    ; switch
    mov eax, 
    ; switch body
case_65:
    ; print
    push 
    call print_uint
    add esp, 4
	jmp end_switch
case_70:
    ; print
    push 
    call print_uint
    add esp, 4
	jmp end_switch
end_switch:
    ; Объявление переменной word
    ; switch
    mov eax, 
    ; switch body
case_1751672902:
    ; print
    push 
    call print_uint
    add esp, 4
	jmp end_switch
case_1380931398:
    ; print
    push 
    call print_uint
    add esp, 4
	jmp end_switch
end_switch:
    mov eax, 
    mov esp, ebp
    pop ebp
    ret
end_switch:
main proc
    ; Инициализация консоли
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    mov console_handle, eax
    ; Объявление переменной test_uint
    ; Объявление переменной test_string
    ; Объявление переменной test_char
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    ; print
    push 
    call print_uint
    add esp, 4
    mov eax, 
    mov esp, ebp
    pop ebp
    ret
end_switch:
	invoke ExitProcess, 0
main endp

