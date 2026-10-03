@echo off

: Для x86 (32-битная программа)
ml /c /coff /Fl /Zi asm02.asm

: Параметры:
: /c     - только компиляция, не линковка
: /coff  - формат COFF для Windows
: /Fl    - создать файл листинга
: /Zi    - добавить отладочную информацию


: Линковка с библиотеками

link asm02.obj /subsystem:windows user32.lib

: Параметры:
: /subsystem:windows  - Windows приложение (не консольное)
: /entry:start        - точка входа 
: /debug              - добавить отладочную информацию
: /out:asm02.exe      - имя выходного файла
: kernel32.lib user32.lib - библиотеки Windows API

asm02.exe