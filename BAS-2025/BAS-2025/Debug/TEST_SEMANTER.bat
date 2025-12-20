@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==



BAS-2025.exe -in:test\1.txt> test\LOG.txt
BAS-2025.exe -in:test\2.txt>> test\LOG.txt
BAS-2025.exe -in:test\3.txt>> test\LOG.txt
BAS-2025.exe -in:test\4.txt>> test\LOG.txt
BAS-2025.exe -in:test\5.txt>> test\LOG.txt
BAS-2025.exe -in:test\6.txt>> test\LOG.txt
BAS-2025.exe -in:test\7.txt>> test\LOG.txt
BAS-2025.exe -in:test\8.txt>> test\LOG.txt
BAS-2025.exe -in:test\9.txt>> test\LOG.txt
BAS-2025.exe -in:test\10.txt>> test\LOG.txt
BAS-2025.exe -in:test\11.txt>> test\LOG.txt
BAS-2025.exe -in:test\12.txt>> test\LOG.txt
BAS-2025.exe -in:test\13.txt>> test\LOG.txt
BAS-2025.exe -in:test\14.txt>> test\LOG.txt
BAS-2025.exe -in:test\15.txt>> test\LOG.txt
BAS-2025.exe -in:test\16.txt>> test\LOG.txt
BAS-2025.exe -in:test\17.txt>> test\LOG.txt
BAS-2025.exe -in:test\18.txt>> test\LOG.txt
BAS-2025.exe -in:test\19.txt>> test\LOG.txt
BAS-2025.exe -in:test\20.txt>> test\LOG.txt



setlocal enabledelayedexpansion
find /i "ERROR:" test\1.txt.log > test\ERR.txt
find /i "ERROR:" test\13.txt.log >> test\ERR.txt
find /i "ERROR:" test\12.txt.log >> test\ERR.txt
find /i "ERROR:" test\2.txt.log >> test\ERR.txt
find /i "ERROR:" test\11.txt.log >> test\ERR.txt
find /i "ERROR:" test\14.txt.log >> test\ERR.txt
find /i "ERROR:" test\3.txt.log >> test\ERR.txt
find /i "ERROR:" test\4.txt.log >> test\ERR.txt
find /i "ERROR:" test\6.txt.log >> test\ERR.txt
find /i "ERROR:" test\5.txt.log >> test\ERR.txt
find /i "ERROR:" test\7.txt.log >> test\ERR.txt
find /i "ERROR:" test\18.txt.log >> test\ERR.txt
find /i "ERROR:" test\8.txt.log >> test\ERR.txt
find /i "ERROR:" test\10.txt.log >> test\ERR.txt
find /i "ERROR:" test\9.txt.log >> test\ERR.txt
find /i "ERROR:" test\19.txt.log >> test\ERR.txt
find /i "ERROR:" test\20.txt.log >> test\ERR.txt
find /i "ERROR:" test\17.txt.log >> test\ERR.txt
find /i "ERROR:" test\16.txt.log >> test\ERR.txt
find /i "ERROR:" test\15.txt.log >> test\ERR.txt



type test\ERR.txt
echo == END ==