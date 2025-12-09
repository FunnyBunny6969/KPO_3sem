@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

PolishNotation.exe -in:in.txt

echo == OUT and LOG ==
type in.txt.out && type in.txt.log
pause