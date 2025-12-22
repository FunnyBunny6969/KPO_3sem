@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==



for /l %%i in (1,1,20) do (
    BAS-2025.exe -in:test\%%i.txt -log:test\LOG%%i.txt
)



goto :skip
setlocal enabledelayedexpansion
find /i "ERROR:" test\LOG1.txt > test\ERR.txt
find /i "ERROR:" test\LOG13.txt >> test\ERR.txt
find /i "ERROR:" test\LOG12.txt >> test\ERR.txt
find /i "ERROR:" test\LOG2.txt >> test\ERR.txt
find /i "ERROR:" test\LOG11.txt >> test\ERR.txt
find /i "ERROR:" test\LOG14.txt >> test\ERR.txt
find /i "ERROR:" test\LOG3.txt >> test\ERR.txt
find /i "ERROR:" test\LOG4.txt >> test\ERR.txt
find /i "ERROR:" test\LOG6.txt >> test\ERR.txt
find /i "ERROR:" test\LOG5.txt >> test\ERR.txt
find /i "ERROR:" test\LOG7.txt >> test\ERR.txt
find /i "ERROR:" test\LOG18.txt >> test\ERR.txt
find /i "ERROR:" test\LOG8.txt >> test\ERR.txt
find /i "ERROR:" test\LOG10.txt >> test\ERR.txt
find /i "ERROR:" test\LOG9.txt >> test\ERR.txt
find /i "ERROR:" test\LOG19.txt >> test\ERR.txt
find /i "ERROR:" test\LOG20.txt >> test\ERR.txt
find /i "ERROR:" test\LOG17.txt >> test\ERR.txt
find /i "ERROR:" test\LOG16.txt >> test\ERR.txt
find /i "ERROR:" test\LOG15.txt >> test\ERR.txt

type test\ERR.txt
:skip


echo == END ==