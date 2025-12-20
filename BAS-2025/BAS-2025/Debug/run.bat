@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

BAS-2025.exe -in:program.txt

echo == OUT and LOG ==
type *.txt.out && type *.txt.log