@echo off
chcp 1251 > nul
echo == SET Windows-1251 ==
LexRec.exe -in:in.txt && type in.txt.out && type in.txt.log
pause