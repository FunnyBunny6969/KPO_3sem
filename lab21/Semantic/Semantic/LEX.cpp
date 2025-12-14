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

        CollectFunctionMetadata(lexTable, idTable);
    }



    void CollectFunctionMetadata(LT::LexTable& lexTable, IT::IdTable& idTable)
    {
        // Итерация по Таблице Идентификаторов (ТИ)
        for (int i = 0; i < idTable.size; i++) {
            IT::Entry& currentId = idTable.table[i];

            // 1. Ищем только записи функций
            if (currentId.idtype == IT::F) {


                // Индекс первой лексемы, относящейся к функции
                int lexIndex = currentId.idxfirstLE;
                if (lexIndex == LT_TI_NULLIDX) continue;
                if (lexIndex < 0 || lexIndex >= lexTable.size) continue;

                // Инициализация метаданных для текущей функции
                currentId.func_meta.n_params = 0;
                bool inFunctionParams = false;
                bool functionBodyFound = false;

                // 2. Сканируем Таблицу Лексем от начала функции
                for (int j = lexIndex; j < lexTable.size; j++) {
                    char code = lexTable.table[j].lexema[0];

                    if (code == LEX_LEFTHESIS) {
                        inFunctionParams = true; // Начались параметры
                        continue;
                    }

                    if (code == LEX_RIGHTHESIS) {
                        inFunctionParams = false; // Закончились параметры
                        continue;
                    }

                    if (code == LEX_LEFTBRACE) {
                        // Начало тела функции
                        inFunctionParams = false; // Убеждаемся, что флаг сброшен
                        functionBodyFound = true;

                        int braceDepth = 1;
                        // Новый цикл для поиска конца тела функции
                        for (int k = j + 1; k < lexTable.size; k++) {
                            if (lexTable.table[k].lexema[0] == LEX_LEFTBRACE) {
                                braceDepth++;
                            }
                            else if (lexTable.table[k].lexema[0] == LEX_RIGHTBRACE) {
                                braceDepth--;
                                if (braceDepth == 0) {
                                    // Найдена закрывающая скобка функции. 
                                    // Устанавливаем j на позицию '{' (чтобы внешний цикл 
                                    // сразу вышел на следующей итерации или был пропущен,
                                    // но поскольку мы прерываем внешний цикл, это не так важно,
                                    // просто прерываем оба цикла).
                                    j = lexTable.size; // Чтобы выйти из внешнего цикла
                                    break; // Выход из внутреннего цикла
                                }
                            }
                        }
                        // Если мы нашли тело функции, то после внутреннего цикла
                        // мы должны выйти и из внешнего, чтобы перейти к следующей функции.
                        // Если `j` было установлено в `lexTable.size`, внешний цикл завершится.
                        break;
                    }

                    // 3. Собираем параметры внутри скобок
                    if (inFunctionParams && code == LEX_ID) {
                        int idIndex = lexTable.table[j].idxTI;

                        // Проверяем, является ли этот ID параметром (IT::P)
                        if (idIndex != LT_TI_NULLIDX && idTable.table[idIndex].idtype == IT::P) {

                            if (currentId.func_meta.n_params < IT_MAX_PARAMS) {

                                // Сохраняем тип параметра в метаданных функции
                                IT::IDDATATYPE paramType = idTable.table[idIndex].iddatatype;

                                currentId.func_meta.params_types[currentId.func_meta.n_params] = paramType;
                                currentId.func_meta.n_params++;
                            }
                            else {
                                std::cout << "ERR: TOO MUCH PARMS in func"
                                    << currentId.value.vstr << std::endl;
                            }
                        }
                    }

                    // Обработка функций-объявлений (без тела)
                    if (!functionBodyFound && code == LEX_SEMICOLON) {
                        // Если мы нашли ';' после параметров, это объявление функции.
                        // Мы можем завершить сканирование для этой функции.
                        break;
                    }
                }
            }
        }
    }
}