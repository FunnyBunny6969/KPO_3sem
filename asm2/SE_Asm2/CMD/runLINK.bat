@echo off

:: Линковка с библиотеками
link.exe asm02.obj /subsystem:windows /entry:start /debug /out:asm02.exe kernel32.lib user32.lib

:: Параметры:
:: /subsystem:windows  - Windows приложение (не консольное)
:: /entry:start        - точка входа 
:: /debug              - добавить отладочную информацию
:: /out:asm02.exe      - имя выходного файла
:: kernel32.lib user32.lib - библиотеки Windows API