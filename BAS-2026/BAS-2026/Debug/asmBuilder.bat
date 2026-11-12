@echo off
chcp 65001 > nul

call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvars32.bat" > nul
if errorlevel 1 (
    echo [ОШИБКА] Не удалось загрузить окружение Visual Studio
    pause
    exit /b 1
)

set ASM_NAME=program

echo === 1. Компиляция %ASM_NAME%.asm в %ASM_NAME%.obj ===
ml.exe /c /coff /Cp %ASM_NAME%.asm
if errorlevel 1 (
    echo [ОШИБКА] Ошибка в коде ассемблера ml.exe
    pause
    exit /b 1
)

echo === 2. Линковка %ASM_NAME%.obj + StdLib.lib в %ASM_NAME%.exe ===
link.exe /SUBSYSTEM:CONSOLE %ASM_NAME%.obj StdLib.lib kernel32.lib ucrtd.lib vcruntimed.lib msvcrtd.lib legacy_stdio_definitions.lib /NODEFAULTLIB:libucrt.lib /OUT:%ASM_NAME%.exe
if errorlevel 1 (
    echo [ОШИБКА] Ошибка линковки link.exe
    pause
    exit /b 1
)

echo === 3. Запуск скомпилированной программы %ASM_NAME%.exe ===
echo ---------------------------------------------------
%ASM_NAME%.exe
set EXIT_CODE=%ERRORLEVEL%
chcp 65001 > nul
echo ---------------------------------------------------
echo Код возврата программы: %EXIT_CODE%
pause