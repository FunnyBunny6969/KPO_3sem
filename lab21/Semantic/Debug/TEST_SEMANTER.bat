@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==



Semantic.exe -in:test\1.txt> test\LOG.txt
Semantic.exe -in:test\2.txt>> test\LOG.txt
Semantic.exe -in:test\3.txt>> test\LOG.txt
Semantic.exe -in:test\4.txt>> test\LOG.txt
Semantic.exe -in:test\5.txt>> test\LOG.txt
Semantic.exe -in:test\6.txt>> test\LOG.txt
Semantic.exe -in:test\7.txt>> test\LOG.txt
Semantic.exe -in:test\8.txt>> test\LOG.txt
Semantic.exe -in:test\9.txt>> test\LOG.txt
Semantic.exe -in:test\10.txt>> test\LOG.txt
Semantic.exe -in:test\11.txt>> test\LOG.txt
Semantic.exe -in:test\12.txt>> test\LOG.txt
Semantic.exe -in:test\13.txt>> test\LOG.txt
Semantic.exe -in:test\14.txt>> test\LOG.txt
Semantic.exe -in:test\15.txt>> test\LOG.txt
Semantic.exe -in:test\16.txt>> test\LOG.txt
Semantic.exe -in:test\17.txt>> test\LOG.txt
Semantic.exe -in:test\18.txt>> test\LOG.txt
Semantic.exe -in:test\19.txt>> test\LOG.txt
Semantic.exe -in:test\20.txt>> test\LOG.txt



setlocal enabledelayedexpansion
find /i "Îøèáêà" test\1.txt.log > test\ERR.txt
find /i "Îøèáêà" test\13.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\12.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\2.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\11.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\14.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\3.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\4.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\6.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\5.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\7.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\18.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\8.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\10.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\9.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\19.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\20.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\17.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\16.txt.log >> test\ERR.txt
find /i "Îøèáêà" test\15.txt.log >> test\ERR.txt



type test\ERR.txt
echo == END ==