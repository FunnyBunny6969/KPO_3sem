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
                lines.push_back(currentLine);
                currentLine.clear();
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


    int CheckLiteralByValue(const IT::IdTable& idTable, const IT::Entry& newEntry) {
        if (newEntry.idtype != IT::L) {
            return TI_NULLIDX;
        }

        for (int i = 0; i < idTable.size; i++) {
            const IT::Entry& existingEntry = idTable.table[i];

            // 1. Проверяем, что существующая запись тоже является литералом (IT::L)
            if (existingEntry.idtype != IT::L) {
                continue;
            }

            // 2. Проверяем, что типы данных совпадают (INT, STR, CHAR)
            if (existingEntry.iddatatype != newEntry.iddatatype) {
                continue;
            }


            // ЧИСЛОВОЙ ЛИТЕРАЛ
            if (newEntry.iddatatype == IT::UINT) {
                if (existingEntry.value.vint == newEntry.value.vint) {
                    return i;
                }
            }

            else if (newEntry.iddatatype == IT::CHAR) {
                if ((int)existingEntry.value.vchar == (int)newEntry.value.vchar) {
                    return i;
                }
            }

            // СТРОКОВЫЙ ЛИТЕРАЛ (STR)
            else if (newEntry.iddatatype == IT::STR) {
                if (existingEntry.value.vstr[0].len == newEntry.value.vstr[0].len) {
                    if (memcmp(existingEntry.value.vstr[0].str,
                        newEntry.value.vstr[0].str,
                        newEntry.value.vstr[0].len) == 0)
                    {
                        return i;
                    }
                }
            }
        }

        return TI_NULLIDX;
    }


    long long HexToDemical(const std::string& word) {
        try {
            return std::stoll(word, nullptr, 0);
        }
        catch (const Error::ERROR& e) {
            throw ERROR_THROW(205);
        }
    }



    void Analyze(const char* sourceCode, LT::LexTable& lexTable, IT::IdTable& idTable) {

        std::vector<std::vector<std::string>> lines;
        SplitIntoWords(sourceCode, lines);

        int lineNumber = 1;
        int wordNumber = 1;
        int literalCount = 1;

        std::vector<int> scope;
        scope.push_back(GLOBAL_SCOPE);
        int reserve = 0;

        IT::IDTYPE currentIdType = IT::V;
        IT::IDDATATYPE currentDataType = IT::UNDEF;
        LT::Entry lexEntry;
        bool isDeclaration = false;

        for (const auto& line : lines) {
            for (const auto& word : line) {
                char code = Automata::getLexemeCode(word.c_str());
                // Создаем запись для таблицы лексем
                lexEntry.lexema[0] = code;
                lexEntry.lexema[1] = IN_CODE_ENDS;
                lexEntry.sn = lineNumber;

                if (code == LEX_UNKNOWN) { throw ERROR_THROW_IN(200, lineNumber, wordNumber); }

                else if (code == LEX_MAIN) {
                    scope.push_back(MAIN_SCOPE);
                }


                else if (code == LEX_FUNCTION) {
                    currentIdType = IT::F;
                }


                else if (code == LEX_LEFTHESIS && currentIdType == IT::F) {
                    currentIdType = IT::P;
                }
                else if (code == LEX_RIGHTHESIS && currentIdType == IT::P) {
                    currentIdType = IT::V;
                }


                else if (code == LEX_SWITCH) {
                    reserve++;
                }
                else if (code == LEX_RIGHTBRACE) {
                    if (reserve != 0) reserve--;
                    else {
                        scope.pop_back();
                    }
                }


                else if (code == LEX_UINT) // int/char/string
                {
                    if (Automata::executeAutomata(Automata::U_INTEGER, word.c_str())) currentDataType = IT::UINT;
                    else if (Automata::executeAutomata(Automata::CHAR, word.c_str())) currentDataType = IT::CHAR;
                    else currentDataType = IT::STR;
                    isDeclaration = true;
                }


                else if (code == LEX_ID) {
                    int idIndex = TI_NULLIDX;

                    if (isDeclaration) {
                        IT::Entry idEntry;

                        int copyLen = word.length();
                        if (copyLen >= ID_MAXSIZE) {
                            copyLen = ID_MAXSIZE - 1;
                            throw ERROR_THROW(201);
                        }
                        for (int i = 0; i < copyLen; i++) {
                            idEntry.id[i] = word[i];
                        }
                        idEntry.id[copyLen] = IN_CODE_ENDS;


                        idEntry.idtype = currentIdType;
                        idEntry.iddatatype = currentDataType;
                        idEntry.value.vint = TI_INT_DEFAULT;
                        idEntry.idxfirstLE = lexTable.size;
                        idEntry.scope = scope.back();


                        IT::Add(idTable, idEntry);
                        idIndex = idTable.size - 1;
                        if (idEntry.idtype == IT::F) {
                            scope.push_back(idIndex);
                            reserve = 0;
                        }

                        if (idEntry.idtype == IT::P) {
                            int num = idTable.table[scope.back()].func_meta.n_params;
                            idTable.table[scope.back()].func_meta.params_types[num] = idEntry.iddatatype;
                            idTable.table[scope.back()].func_meta.n_params++;
                        }
                    }

                    else {
                        IT::Entry idEntry;

                        int copyLen = word.length();
                        if (copyLen >= ID_MAXSIZE) {
                            copyLen = ID_MAXSIZE - 1;
                            throw ERROR_THROW(201);
                        }
                        for (int i = 0; i < copyLen; i++) {
                            idEntry.id[i] = word[i];
                        }
                        idEntry.id[copyLen] = IN_CODE_ENDS;
                        idEntry.value.vint = TI_INT_DEFAULT;
                        idEntry.scope = scope.back();

                        idIndex = IT::IsId(idTable, idEntry.id, idEntry.scope);
                        if (idIndex == TI_NULLIDX) {
                            throw ERROR_THROW_LINE(703, lexEntry.sn);
                        }
                    }

                    lexEntry.idxTI = idIndex;
                    isDeclaration = false;
                }


                else if (code == LEX_LITERAL) {
                    int idIndex = TI_NULLIDX;

                    IT::Entry idEntry;
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
                        idEntry.iddatatype = IT::UINT;
                        idEntry.value.vint = std::stoi(word);
                    }
                    // Числовой литерал HEX
                    else if (Automata::executeAutomata(Automata::HEX_NUMBER_LITERAL, word.c_str())) {
                        idEntry.iddatatype = IT::UINT;
                        try { idEntry.value.vint = HexToDemical(word); }
                        catch (const Error::ERROR& e) { throw e; }
                    }
                    // Символьный литерал
                    else if (Automata::executeAutomata(Automata::CHAR_LITERAL, word.c_str())) {

                        idEntry.iddatatype = IT::CHAR;
                        idEntry.value.vchar = word[1];
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


                    idEntry.idxfirstLE = lexTable.size;
                    idIndex = CheckLiteralByValue(idTable, idEntry);
                    if (idIndex == TI_NULLIDX) {
                        IT::Add(idTable, idEntry);
                        idIndex = idTable.size - 1;
                    }
                    lexEntry.idxTI = idIndex;
                }


                else {
                    lexEntry.idxTI = LT_TI_NULLIDX;
                }

                LT::Add(lexTable, lexEntry);
                wordNumber++;
            }

            lineNumber++;
            wordNumber = 1;
        }
    }
}