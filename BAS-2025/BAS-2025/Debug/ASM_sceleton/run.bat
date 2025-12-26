@echo off

ml.exe /c /coff /Fl /Zi asm.asm
:: Параметры:
:: /c     - только компиляция, не линковка
:: /coff  - формат COFF для Windows
:: /Fl    - создать файл листинга
:: /Zi    - добавить отладочную информацию


link.exe asm.obj ^
	/subsystem:console ^
	/entry:main@0 ^
	/debug ^
	/out:asm.exe ^
	"C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x86\kernel32.lib" ^
	"C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x86\user32.lib"
::	"C:\Program Files (x86)\Windows Kits\10\lib\10.0.26100.0\um\x86\ntstc_msvcrt.lib"

:: Параметры:
:: /subsystem:console  - Windows приложение (не консольное)
:: /entry:main         - точка входа 
:: /debug              - добавить отладочную информацию
:: /out:asm.exe      - имя выходного файла
:: kernel32.lib user32.lib - библиотеки Windows API


echo == RUN ==
asm.exe