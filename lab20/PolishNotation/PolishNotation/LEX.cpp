#include "LEX.h"
#include <sstream>
#include <cctype>
#include <string>
#include <iostream>

namespace LEX {

    void SplitIntoWords(const char* code, std::vector<std::vector<std::string>>& lines) {
        std::vector<std::string> currentLine;
        std::string currentWord;
        bool inString = false;

        for (int i = 0; code[i] != IN_CODE_ENDS; i++) {
            char c = code[i];

            // Обработка строковых литералов
            if (c == QM && !inString) {
                inString = true;
                if (!currentWord.empty()) {
                    currentLine.push_back(currentWord);
                    currentWord.clear();
                }
                currentWord += c;
            }
            else if (c == '\'' && inString) {
                inString = false;
                currentWord += c;
                currentLine.push_back(currentWord);
                currentWord.clear();
            }
            else if (c == SEP && !inString) {
                // Сепаратор | (только если не внутри строки)
                if (!currentWord.empty()) {
                    currentLine.push_back(currentWord);
                    currentWord.clear();
                }
                if (!currentLine.empty()) {
                    lines.push_back(currentLine);
                    currentLine.clear();
                }
            }
            else if ((c == ' ' || c == '\t') && !inString) {
                // Пробел вне строкового литерала - разделитель слов
                if (!currentWord.empty()) {
                    currentLine.push_back(currentWord);
                    currentWord.clear();
                }
            }
            else {
                // Любой другой символ (включая пробелы внутри строки)
                currentWord += c;
            }
        }

        // Обрабатываем последнее слово
        if (!currentWord.empty()) {
            currentLine.push_back(currentWord);
        }

        // Обрабатываем последнюю строку
        if (!currentLine.empty()) {
            lines.push_back(currentLine);
        }
    }


    void Analyze(
        const char* sourceCode,
        LT::LexTable& lexTable,
        IT::IdTable& idTable) {

        std::vector<std::vector<std::string>> lines;
        SplitIntoWords(sourceCode, lines);

        int lineNumber = 1;
        int wordNumber = 1;
        int literalCount = 1;

        // Контекст для определения типов
        std::string currentFunction = "";
        IT::IDTYPE currentIdType = IT::V;
        IT::IDDATATYPE currentDataType = IT::INT;
        bool inParams = false;

        // Проходим по всем строкам
        for (const auto& line : lines) {
            // Проходим по всем словам в строке
            for (const auto& word : line) {
                // Пропускаем слово через все автоматы
                char code = Automata::getLexemeCode(word.c_str());

                // Обработка ошибок
                if (code == '?') {
                    throw ERROR_THROW_IN(200, lineNumber, wordNumber);
                }

                if (code == LEX_FUNCTION) {
                    currentIdType = IT::F;  // следующий ID будет функцией
                }
                else if (code == LEX_INTEGER || code == LEX_STRING) { // integer/string
                    currentDataType = (Automata::executeAutomata(Automata::INTEGER, word.c_str())) ? IT::INT : IT::STR;
                    if (inParams) {
                        currentIdType = IT::P;  // в параметрах - следующий ID будет параметром
                    }
                }
                else if (code == LEX_LEFTHESIS) {
                    inParams = true;  // начались параметры
                }
                else if (code == LEX_RIGHTHESIS) {
                    inParams = false; // закончились параметры
                    currentIdType = IT::V;
                }
                else if (code == LEX_LEFTBRACE) {
                    // Начало тела функции
                    currentIdType = IT::V;
                    inParams = false;
                }
                else if (code == LEX_RIGHTBRACE) {
                    // Конец функции
                    currentFunction = "";
                    currentIdType = IT::V;
                }

                // Создаем запись для таблицы лексем
                LT::Entry lexEntry;
                lexEntry.lexema[0] = code;
                lexEntry.lexema[1] = IN_CODE_ENDS;
                lexEntry.sn = lineNumber;

                // Обрабатываем идентификаторы и литералы
                if (code == LEX_ID || code == LEX_LITERAL) {
                    bool found = false;
                    int idIndex = TI_NULLIDX;

                    // Для литералов всегда создаем новую запись
                    if (code == LEX_LITERAL) {
                        found = false;
                    }
                    else {
                        // Для идентификаторов проверяем существование
                        for (int i = 0; i < idTable.size; i++) {
                            if (strcmp(idTable.table[i].id, word.c_str()) == 0) {
                                found = true;
                                idIndex = i;
                                break;
                            }
                        }


                    }

                    // Если не найден - добавляем в таблицу идентификаторов
                    if (!found) {
                        IT::Entry idEntry;

                        if (code == LEX_ID) {



                            // ИДЕНТИФИКАТОР - обрезаем до 5 символов
                            int copyLen = word.length();
                            if (copyLen >= ID_MAXSIZE)
                                copyLen = ID_MAXSIZE - 1;

                            for (int i = 0; i < copyLen; i++) {
                                idEntry.id[i] = word[i];
                            }
                            idEntry.id[copyLen] = IN_CODE_ENDS;

                            // ИСПОЛЬЗУЕМ КОНТЕКСТ ДЛЯ ОПРЕДЕЛЕНИЯ ТИПОВ
                            idEntry.idtype = currentIdType;
                            idEntry.iddatatype = currentDataType;
                            idEntry.value.vint = TI_INT_DEFAULT;

                            // После использования сбрасываем контекстные типы
                            if (currentIdType == IT::F) {
                                currentFunction = word; // запоминаем имя функции
                                currentIdType = IT::V;
                            }
                            else if (currentIdType == IT::P) {
                                currentIdType = IT::V;
                            }
                        }
                        else {
                            // ЛИТЕРАЛ - генерируем имя L1, L2, L3...
                            std::string litName = "L" + std::to_string(literalCount++);
                            int copyLen = litName.length();
                            if (copyLen >= ID_MAXSIZE)
                                copyLen = ID_MAXSIZE - 1;

                            for (int i = 0; i < copyLen; i++) {
                                idEntry.id[i] = litName[i];
                            }
                            idEntry.id[copyLen] = IN_CODE_ENDS;

                            idEntry.idtype = IT::L;

                            // Числовой литерал
                            if (Automata::executeAutomata(Automata::NUMBER_LITERAL, word.c_str())) {
                                idEntry.iddatatype = IT::INT;
                                idEntry.value.vint = std::stoi(word);
                            }
                            // Строковый литерал
                            else if (Automata::executeAutomata(Automata::STRING_LITERAL, word.c_str())) {
                                idEntry.iddatatype = IT::STR;
                                // Убираем кавычки и копируем содержимое
                                std::string content = word.substr(1, word.length() - 2);
                                int strLen = content.length();
                                if (strLen >= TI_STR_MAXSIZE - 1)
                                    strLen = TI_STR_MAXSIZE - 2;

                                idEntry.value.vstr[0].len = strLen;
                                for (int i = 0; i < strLen; i++) {
                                    idEntry.value.vstr[0].str[i] = content[i];
                                }
                                idEntry.value.vstr[0].str[strLen] = '\0';
                            }
                        }

                        idEntry.idxfirstLE = lexTable.size;
                        IT::Add(idTable, idEntry);
                        idIndex = idTable.size - 1;
                    }

                    lexEntry.idxTI = idIndex;
                }
                else {
                    lexEntry.idxTI = LT_TI_NULLIDX;
                }

                // Добавляем запись в таблицу лексем
                LT::Add(lexTable, lexEntry);
                wordNumber++;
            }

            lineNumber++;
            wordNumber = 1;
        }

        for (int i = 0; i < lexTable.size; i++) {
            if (lexTable.table[i].lexema[0] == LEX_ID &&
                i + 1 < lexTable.size) {

                char nextLex = lexTable.table[i + 1].lexema[0];

                // Вариант 1: Идентификатор + '(' - это вызов функции
                if (nextLex == LEX_LEFTHESIS) {
                    // НО: проверяем, не внутри ли мы другого вызова?
                    bool insideFunctionCall = false;
                    int parenDepth = 0;

                    // Идём назад от текущей позиции
                    for (int j = i - 1; j >= 0; j--) {
                        if (lexTable.table[j].lexema[0] == LEX_RIGHTHESIS) {
                            parenDepth++;
                        }
                        else if (lexTable.table[j].lexema[0] == LEX_LEFTHESIS) {
                            if (parenDepth == 0) {
                                // Мы внутри вызова функции - это параметр!
                                insideFunctionCall = true;
                                break;
                            }
                            parenDepth--;
                        }
                    }

                    // Если НЕ внутри другого вызова - это самостоятельная функция
                    if (!insideFunctionCall) {
                        int idxTI = lexTable.table[i].idxTI;
                        if (idxTI != LT_TI_NULLIDX && idxTI < idTable.size) {
                            // Меняем только VAR на FUNC
                            if (idTable.table[idxTI].idtype == IT::V) {
                                idTable.table[idxTI].idtype = IT::F;
                            }
                        }
                    }
                }
            }
        }

    }
}