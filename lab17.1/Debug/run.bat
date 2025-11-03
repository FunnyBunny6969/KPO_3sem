@echo off
chcp 1251 > nul
echo == SET Windows-1251 ==
SELab17.exe -in:in.txt
echo == IN ==
type in.txt
echo == OUT ==
type in.txt.out
echo == LOG ==
type in.txt.log
pause