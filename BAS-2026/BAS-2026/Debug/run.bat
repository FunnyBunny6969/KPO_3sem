@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

BAS-2025.exe -in:program.txt -out:program.asm

echo == LOG ==
REM type program.txt.log
type program.txt.log

echo == OUT ==
type program.asm


