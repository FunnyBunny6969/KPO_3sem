; test_program.asm
; Скомпилировать: ml.exe /c /coff test_program.asm
; Слинковать: link test_program.obj kernel32.lib user32.lib /subsystem:console /entry:main

.586
.model flat, stdcall
option casemap:none

; Импорт функций WinAPI
includelib kernel32.lib
includelib user32.lib

GetStdHandle proto :dword
WriteConsoleA proto :dword, :dword, :dword, :dword, :dword
ExitProcess proto :dword
GetTickCount proto
GetProcessHeap proto
HeapAlloc proto :dword, :dword, :dword
HeapFree proto :dword, :dword, :dword

STD_OUTPUT_HANDLE = -11

.data
    ; Строки для вывода
    str_demo_underflow db 'демонстрации переполнения', 13, 10, 0
    str_a db 'a = ', 0
    str_b db 'b = ', 0
    str_c db 'c = ', 0
    str_call_lib db 'Вызов библиотечных функций', 13, 10, 0
    str_pow db 'pow(uint, uint)', 13, 10, 0
    str_random db 'random(uint min, uint max)', 13, 10, 0
    str_bit_test db '=== ТЕСТ БИТОВЫХ ОПЕРАЦИЙ ===', 13, 10, 0
    str_bitwise_ops db 'bitwise_ops(0x12, 0x00) = ', 0
    str_bitwise_ops1 db 'bitwise_ops(0x12, 0x01) = ', 0
    str_bitwise_ops2 db 'bitwise_ops(0x12, 0x02) = ', 0
    str_bitwise_ops3 db 'bitwise_ops(0x12, 0x03) = ', 0
    str_test_uint db 'test_uint = ', 0
    str_test_string db 'test_string = ', 0
    str_test_char db 'test_char = ', 0
    str_before_init db 'ДО', 13, 10, 0
    str_after_init db 'ПОСЛЕ', 13, 10, 0
    str_newline db 13, 10, 0
    str_space db ' ', 0
    
    test_string_literal db 'ТЕСТОВАЯ СТРОКА', 0
    test_char_literal db '@', 0
    
    ; Статические переменные
    random_seed dd 0
    
    ; Глобальные переменные из исходного кода
    test_uint dd 0
    test_string dd 0      ; указатель на строку
    test_char db 0
    
    ; Временные переменные
    console_handle dd 0
    bytes_written dd 0
    
    ; Буфер для преобразования чисел
    num_buffer db 32 dup(0)

.code

; =============== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===============

; Функция print_string: вывод строки в консоль
; [esp+4] = указатель на строку (ASCIIZ)
print_string proc
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi
    
    ; Получаем указатель на строку
    mov esi, [ebp+8]
    
    ; Вычисляем длину строки
    xor ecx, ecx
    strlen_loop:
        cmp byte ptr [esi+ecx], 0
        je strlen_done
        inc ecx
        jmp strlen_loop
    strlen_done:
    
    ; Выводим строку
    invoke WriteConsoleA, console_handle, esi, ecx, offset bytes_written, 0
    
    pop edi
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret 4
print_string endp

; Функция print_uint: вывод беззнакового числа
; [esp+4] = число для вывода
print_uint proc
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
    mov esp, ebp
    pop ebp
    ret 4
print_uint endp

; Функция print_char: вывод символа
; [esp+4] = символ для вывода
print_char proc
    push ebp
    mov ebp, esp
    
    ; Сохраняем символ в буфер
    mov al, byte ptr [ebp+8]
    mov byte ptr [num_buffer], al
    mov byte ptr [num_buffer+1], 0
    
    ; Выводим
    push offset num_buffer
    call print_string
    
    ; Выводим перевод строки
    push offset str_newline
    call print_string
    
    mov esp, ebp
    pop ebp
    ret 4
print_char endp

; Функция pow: возведение в степень
; [esp+4] = основание, [esp+8] = степень
; Возвращает: EAX = результат
pow proc
    push ebp
    mov ebp, esp
    push ebx
    
    mov eax, 1          ; результат
    mov ecx, [ebp+8]    ; основание
    mov edx, [ebp+12]   ; степень
    test edx, edx
    jz pow_done
    
    pow_loop:
        imul eax, ecx
        dec edx
        jnz pow_loop
    
    pow_done:
    pop ebx
    mov esp, ebp
    pop ebp
    ret 8
pow endp

; Функция random: генератор случайных чисел
; [esp+4] = минимум, [esp+8] = максимум
; Возвращает: EAX = случайное число
random proc
    push ebp
    mov ebp, esp
    push ebx
    
    ; Инициализируем seed если нужно
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
    
    mov ecx, [ebp+8]    ; максимум
    mov ebx, [ebp+4]    ; минимум
    sub ecx, ebx
    inc ecx
    xor edx, edx
    div ecx
    mov eax, ebx
    add eax, edx
    
    pop ebx
    mov esp, ebp
    pop ebp
    ret 8
random endp

; =============== ФУНКЦИИ ИЗ ИСХОДНОГО КОДА ===============

; Функция demonstrate_underflow
demonstrate_underflow proc
    push ebp
    mov ebp, esp
    sub esp, 12         ; место для локальных переменных a, b, c
    
    ; Выводим сообщение
    push offset str_demo_underflow
    call print_string
    
    ; a = 5
    mov dword ptr [ebp-4], 5
    push offset str_a
    call print_string
    push [ebp-4]
    call print_uint
    
    ; b = 10
    mov dword ptr [ebp-8], 10
    push offset str_b
    call print_string
    push [ebp-8]
    call print_uint
    
    ; c = a - b (underflow)
    mov eax, [ebp-4]
    sub eax, [ebp-8]
    mov [ebp-12], eax
    push offset str_c
    call print_string
    push [ebp-12]
    call print_uint
    
    mov esp, ebp
    pop ebp
    ret
demonstrate_underflow endp

; Функция use_standard_library
use_standard_library proc
    push ebp
    mov ebp, esp
    
    ; Выводим сообщение
    push offset str_call_lib
    call print_string
    
    ; pow(2, 2)
    push offset str_pow
    call print_string
    push 2      ; степень
    push 2      ; основание
    call pow
    push eax
    call print_uint
    
    ; pow(3, 3)
    push 3      ; степень
    push 3      ; основание
    call pow
    push eax
    call print_uint
    
    ; random(0, 100)
    push offset str_random
    call print_string
    push 100    ; максимум
    push 0      ; минимум
    call random
    push eax
    call print_uint
    
    ; random(1, 9)
    push 9      ; максимум
    push 1      ; минимум
    call random
    push eax
    call print_uint
    
    mov esp, ebp
    pop ebp
    ret
use_standard_library endp

; Функция bitwise_ops
; [esp+4] = result, [esp+8] = op_type
; Возвращает: EAX = результат
bitwise_ops proc
    push ebp
    mov ebp, esp
    push ebx
    
    mov eax, [ebp+8]    ; op_type
    mov ebx, [ebp+4]    ; result
    
    ; Switch по op_type
    cmp eax, 0
    je case_0
    cmp eax, 1
    je case_1
    cmp eax, 2
    je case_2
    jmp default_case
    
    case_0:    ; AND с 0x0F
        and ebx, 0Fh
        jmp end_switch
    
    case_1:    ; OR с 0xF0
        or ebx, 0F0h
        jmp end_switch
    
    case_2:    ; NOT
        not ebx
        jmp end_switch
    
    default_case:   ; умножить на 2
        shl ebx, 1
    
    end_switch:
    mov eax, ebx
    
    pop ebx
    mov esp, ebp
    pop ebp
    ret 8
bitwise_ops endp

; =============== ГЛАВНАЯ ФУНКЦИЯ ===============
main proc
    ; Инициализация
    invoke GetStdHandle, STD_OUTPUT_HANDLE
    mov console_handle, eax
    
    ; Проверка значений ДО инициализации
    push offset str_before_init
    call print_string
    
    ; test_uint (неинициализирован)
    push offset str_test_uint
    call print_string
    push test_uint
    call print_uint
    
    ; test_string (неинициализирован)
    push offset str_test_string
    call print_string
    cmp test_string, 0
    je empty_string
    push test_string
    call print_string
    jmp after_string
empty_string:
    push offset str_newline
    call print_string
after_string:
    
    ; test_char (неинициализирован)
    push offset str_test_char
    call print_string
    movzx eax, test_char
    push eax
    call print_char
    
    ; Инициализация
    mov test_uint, 5
    mov test_string, offset test_string_literal
    mov test_char, '@'
    
    ; Проверка значений ПОСЛЕ инициализации
    push offset str_after_init
    call print_string
    
    ; test_uint
    push offset str_test_uint
    call print_string
    push test_uint
    call print_uint
    
    ; test_string
    push offset str_test_string
    call print_string
    push test_string
    call print_string
    
    ; test_char
    push offset str_test_char
    call print_string
    movzx eax, test_char
    push eax
    call print_char
    
    ; Демонстрация underflow
    call demonstrate_underflow
    
    ; Вызовы стандартной библиотеки
    call use_standard_library
    
    ; Тест битовых операций
    push offset str_bit_test
    call print_string
    
    ; bitwise_ops(0x12, 0x00)
    push offset str_bitwise_ops
    call print_string
    push 0      ; op_type
    push 12h    ; result
    call bitwise_ops
    push eax
    call print_uint
    
    ; bitwise_ops(0x12, 0x01)
    push offset str_bitwise_ops1
    call print_string
    push 1      ; op_type
    push 12h    ; result
    call bitwise_ops
    push eax
    call print_uint
    
    ; bitwise_ops(0x12, 0x02)
    push offset str_bitwise_ops2
    call print_string
    push 2      ; op_type
    push 12h    ; result
    call bitwise_ops
    push eax
    call print_uint
    
    ; bitwise_ops(0x12, 0x03)
    push offset str_bitwise_ops3
    call print_string
    push 3      ; op_type
    push 12h    ; result
    call bitwise_ops
    push eax
    call print_uint
    
    ; Завершение программы
    invoke ExitProcess, 0
main endp

end main