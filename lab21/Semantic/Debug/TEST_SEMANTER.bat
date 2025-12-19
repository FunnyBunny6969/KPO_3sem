@echo off
cls

chcp 1251 > nul
echo == SET Windows-1251 ==

Semantic.exe -in:test\1.txt> test\LOG.txt
Semantic.exe -in:test\2.txt>> test\LOG.txt
Semantic.exe -in:test\3.txt> test\LOG.txt
Semantic.exe -in:test\4.txt>> test\LOG.txt
Semantic.exe -in:test\5.txt>> test\LOG.txt
Semantic.exe -in:test\6.txt>> test\LOG.txt
Semantic.exe -in:test\7.txt>> test\LOG.txt
Semantic.exe -in:test\8.txt>> test\LOG.txt
Semantic.exe -in:test\9.txt>> test\LOG.txt
Semantic.exe -in:test\10.txt>> test\LOG.txt


setlocal enabledelayedexpansion
find /i "Îøèáêà" test\1.txt.log > errors.txt
find /i "Îøèáêà" test\2.txt.log >> errors.txt
find /i "Îøèáêà" test\3.txt.log >> errors.txt
find /i "Îøèáêà" test\4.txt.log >> errors.txt
find /i "Îøèáêà" test\5.txt.log >> errors.txt
find /i "Îøèáêà" test\6.txt.log >> errors.txt
find /i "Îøèáêà" test\7.txt.log >> errors.txt
find /i "Îøèáêà" test\8.txt.log >> errors.txt
find /i "Îøèáêà" test\9.txt.log >> errors.txt
find /i "Îøèáêà" test\10.txt.log >> errors.txt


type errors.txt
echo == END ==