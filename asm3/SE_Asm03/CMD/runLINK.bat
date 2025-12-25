@echo off

:: Линковка с библиотеками
link.exe asm3.obj ^
	/subsystem:console ^
	/entry:main ^
	/debug ^
	/out:asm3.exe ^
	"C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x86\kernel32.lib" ^
	"C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x86\user32.lib"

:: Параметры:
:: /subsystem:windows  - Windows приложение (не консольное)
:: /entry:start        - точка входа 
:: /debug              - добавить отладочную информацию
:: /out:asm02.exe      - имя выходного файла
:: kernel32.lib user32.lib - библиотеки Windows API