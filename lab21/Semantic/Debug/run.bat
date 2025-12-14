@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

Semantic.exe -in:program.txt

echo == OUT and LOG ==
type program.txt.out && type program.txt.log
pause