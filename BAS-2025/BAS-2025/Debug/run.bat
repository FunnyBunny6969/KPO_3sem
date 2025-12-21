@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

BAS-2025.exe -in:program.txt -out:program.js

echo == OUT and LOG ==
type program.js && type program.txt.log


echo == RUN JS ==
node program.js