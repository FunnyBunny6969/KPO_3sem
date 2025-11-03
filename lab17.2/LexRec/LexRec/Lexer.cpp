#define _CRT_SECURE_NO_WARNINGS  
#include "Lexer.h"
#include "Automata.h"
#include <iostream>
#include <sstream>
#include <cctype>

namespace Lexer {

    void SplitIntoWords(const char* code, std::vector<std::string>& words) {
        std::string currentWord;
        bool inString = false;

        for (int i = 0; code[i] != '\0'; i++) {
            char c = code[i];

            // Обработка строковых литералов
            if (c == '\'' && !inString) {
                inString = true;
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
                currentWord += c;
            }
            else if (c == '\'' && inString) {
                inString = false;
                currentWord += c;
                words.push_back(currentWord);
                currentWord.clear();
            }
            else if (inString) {
                currentWord += c;
            }
            // Обработка разделителей
            else if (IsDelimiter(c)) {
                if (!currentWord.empty()) {
                    words.push_back(currentWord);
                    currentWord.clear();
                }
                if (!std::isspace(c)) {
                    words.push_back(std::string(1, c));
                }
            }
            else {
                currentWord += c;
            }
        }

        // Последнее слово
        if (!currentWord.empty()) {
            words.push_back(currentWord);
        }
    }

    bool IsDelimiter(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' ||
            c == '(' || c == ')' || c == '{' || c == '}' ||
            c == ';' || c == ',' || c == '=' || c == '+' ||
            c == '-' || c == '*' || c == '/';
    }

    char GetTokenCode(const std::string& text) {
        // Ключевые слова
        if (text == "integer") return 't';
        if (text == "string") return 't';
        if (text == "function") return 'f';
        if (text == "declare") return 'd';
        if (text == "return") return 'r';
        if (text == "print") return 'p';
        if (text == "main") return 'm';

        // Идентификаторы
        if (Automata::executeAutomata(Automata::IDENTIFIER, text.c_str())) {
            return 'i';
        }

        // Литералы
        if (Automata::executeAutomata(Automata::NUMBER_LITERAL, text.c_str())) {
            return 'l';
        }
        if (Automata::executeAutomata(Automata::STRING_LITERAL, text.c_str())) {
            return 'l';
        }

        // Операторы и разделители
        if (text == ";") return ';';
        if (text == ",") return ',';
        if (text == "(") return '(';
        if (text == ")") return ')';
        if (text == "{") return '{';
        if (text == "}") return '}';
        if (text == "=") return '=';
        if (text == "+") return '+';
        if (text == "-") return '-';
        if (text == "*") return '*';
        if (text == "/") return '/';

        return '?';
    }

    void Analyze(
        const char* sourceCode,
        LT::LexTable& lexTable,
        IT::IdTable& idTable,
        std::vector<Error::ERROR>& errors) {

        std::vector<std::string> words;
        SplitIntoWords(sourceCode, words);

        int lineNumber = 1;
        int position = 1;

        for (const auto& word : words) {
            char code = GetTokenCode(word);

            // Добавляем в таблицу лексем
            LT::Entry lexEntry;
            lexEntry.lexema[0] = code;
            lexEntry.lexema[1] = '\0';
            lexEntry.sn = lineNumber;

            // Для идентификаторов добавляем в таблицу идентификаторов
            if (code == 'i') {
                int idIndex = IT::IsId(idTable, (char*)word.c_str());
                if (idIndex == TI_NULLIDX) {
                    IT::Entry idEntry;
                    strncpy(idEntry.id, word.c_str(), ID_MAXSIZE - 1);
                    idEntry.id[ID_MAXSIZE - 1] = '\0'; // гарантированно завершаем строку
                    idEntry.idxfirstLE = lexTable.size;
                    idEntry.iddatatype = IT::INT;
                    idEntry.idtype = IT::V;
                    idEntry.value.vint = TI_INT_DEFAULT;
                    IT::Add(idTable, idEntry);
                    idIndex = idTable.size - 1;
                }
                lexEntry.idxTI = idIndex;
            }
            else {
                lexEntry.idxTI = LT_TI_NULLIDX;
            }

            LT::Add(lexTable, lexEntry);

            // Обновляем позицию (упрощенно)
            position += word.length();

            // Счетчик строк (упрощенно)
            if (word == "\n") {
                lineNumber++;
                position = 1;
            }
        }
    }

    void PrintLexemeTable(const LT::LexTable& lexTable) {
        std::cout << "=== ТАБЛИЦА ЛЕКСЕМ ===" << std::endl;

        int currentLine = 1;
        bool firstInLine = true;

        for (int i = 0; i < lexTable.size; i++) {
            LT::Entry entry = LT::GetEntry((LT::LexTable&)lexTable, i);

            if (entry.sn != currentLine) {
                std::cout << std::endl;
                currentLine = entry.sn;
                firstInLine = true;
            }

            if (firstInLine) {
                if (currentLine < 10) std::cout << "0";
                std::cout << currentLine << " ";
                firstInLine = false;
            }

            std::cout << entry.lexema[0];
        }
        std::cout << std::endl;
    }

    void PrintIdentifierTable(const IT::IdTable& idTable, const LT::LexTable& lexTable) {
        std::cout << "\n=== ТАБЛИЦА ИДЕНТИФИКАТОРОВ ===" << std::endl;
        std::cout << "Индекс | Идентификатор | Первое вхождение" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        for (int i = 0; i < idTable.size; i++) {
            IT::Entry entry = IT::GetEntry((IT::IdTable&)idTable, i);
            std::cout << i << "\t| " << entry.id << "\t| " << entry.idxfirstLE << std::endl;
        }
    }
}