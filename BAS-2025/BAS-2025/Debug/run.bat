@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

BAS-2025.exe -in:program.txt -out:program.js -log:program.log

echo == LOG ==
type program.log

echo == OUT ==
type program.js

echo: 
echo: 
echo:
echo =============== RUN JS =============== 
echo:
echo:
echo:
node program.js



