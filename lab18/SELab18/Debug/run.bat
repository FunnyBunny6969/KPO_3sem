@echo off
cls
chcp 1251 > nul
echo == SET Windows-1251 ==
SELab18.exe -in:in.txt
echo == OUT and LOG ==
type in.txt.out && type in.txt.log
pause