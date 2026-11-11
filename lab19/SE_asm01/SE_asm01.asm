.586P
.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib user32.lib ; Обязательно для MessageBoxA

ExitProcess PROTO : DWORD
MessageBoxA PROTO : DWORD, : DWORD, : DWORD, : DWORD

.STACK 4096

.CONST 
Arr sdword 6,4,6,8,2,5,6,8,9,10

.DATA
str1 DB "Lab_19", 0
; Зарезервировали место под 5 символов числа и нуль-терминатор
str2 DB "мин = ", 0 

HW DWORD 0

.CODE

main PROC
START:

; Передаем параметры: сначала размер, затем адрес
push lengthof Arr
push offset Arr
call getMin ; Результат (минимальное число 2) вернется в EAX

; Подготовка к конвертации числа в строку
mov ecx, 10                                
lea edi, [str2 + 11] ; Указываем на предпоследний байт строки (перед 0)

ConvertToString:
xor edx, edx         ; ОБЯЗАТЕЛЬНО ОБНУЛЯТЬ EDX ВНУТРИ ЦИКЛА
div ecx              ; Делим EDX:EAX на 10. Частное в EAX, остаток в EDX
add dl, '0'          ; Превращаем остаток в символ ('0'-'9')
mov [edi], dl        ; Записываем в строку
dec edi              ; Сдвигаемся влево
test eax, eax        ; Проверяем, осталось ли что-то в EAX
jnz ConvertToString

; Выводим результат
INVOKE MessageBoxA, HW, offset str2, offset str1, 0

push 0
call ExitProcess
main ENDP


; Функция поиска минимума
getMin PROC uses ecx esi ebx, arrOffset: dword, arrLength: dword
      mov   ecx, arrLength   ; Количество элементов
      mov   esi, arrOffset   ; Адрес начала массива
      mov   eax, [esi]       ; Переносим первый элемент в EAX (считаем его минимумом)
      
      ; Если в массиве 1 элемент, можно сразу выходить
      cmp   ecx, 1
      jle   DONE

CYCLE: 
      mov   ebx, [esi]       ; Читаем текущий элемент
      cmp   ebx, eax         ; Сравниваем текущий с минимальным (EAX)
      jge   NOTMIN           ; Если текущий >= минимума, пропускаем
      mov   eax, ebx         ; Если текущий меньше, то он становится новым минимумом
      
NOTMIN: 
      add   esi, 4           ; Переходим к следующему элементу (4 байта для sdword)
      loop  CYCLE            ; Повторяем для всех элементов

DONE:
      ret
getMin ENDP

END main
