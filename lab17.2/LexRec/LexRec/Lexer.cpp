#define _CRT_SECURE_NO_WARNINGS  
#include "Lexer.h"
#include <sstream>
#include <cctype>
#include <string>
#include <iostream>

namespace Lexer {
    void SplitIntoWords(const char* code, std::vector<std::vector<std::string>>& lines) {
        std::string currentLine;
        bool inString = false;

        for (int i = 0; code[i] != '\0'; i++) {
            char c = code[i];

            // Обработка строковых литералов
            if (c == '\'' && !inString) {
                inString = true;
                currentLine += c;
            }
            else if (c == '\'' && inString) {
                inString = false;
                currentLine += c;
            }
            else if (c == '|' && !inString) {
                // Сепаратор | (только если не внутри строки)
                if (!currentLine.empty()) {
                    // Разбиваем текущую строку на слова
                    std::vector<std::string> wordsInLine;
                    std::stringstream lineSS(currentLine);
                    std::string word;

                    while (lineSS >> word) {
                        wordsInLine.push_back(word);
                    }

                    if (!wordsInLine.empty()) {
                        lines.push_back(wordsInLine);
                    }
                    currentLine.clear();
                }
            }
            else {
                // Любой другой символ (включая | внутри строки)
                currentLine += c;
            }
        }

        // Обрабатываем последнюю строку
        if (!currentLine.empty()) {
            std::vector<std::string> wordsInLine;
            std::stringstream lineSS(currentLine);
            std::string word;

            while (lineSS >> word) {
                wordsInLine.push_back(word);
            }

            if (!wordsInLine.empty()) {
                lines.push_back(wordsInLine);
            }
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

				// Создаем запись для таблицы лексем
				LT::Entry lexEntry;
				lexEntry.lexema[0] = code;
				lexEntry.lexema[1] = '\0';
				lexEntry.sn = lineNumber;

				// Обрабатываем идентификаторы
				if (code == LEX_ID) {
					// Проверяем, есть ли уже такой идентификатор
					bool found = false;
					int idIndex = TI_NULLIDX;

					for (int i = 0; i < idTable.size; i++) {
						if (strcmp(idTable.table[i].id, word.c_str()) == 0) {
							found = true;
							idIndex = i;
							break;
						}
					}

					// Если идентификатор новый - добавляем в таблицу идентификаторов
					if (!found) {
						IT::Entry idEntry;

						// Безопасное копирование идентификатора
						int copyLen = word.length();
						if (copyLen >= ID_MAXSIZE)
							copyLen = ID_MAXSIZE - 1;

						for (int i = 0; i < copyLen; i++) {
							idEntry.id[i] = word[i];
						}
						idEntry.id[copyLen] = '\0';

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

				// Добавляем запись в таблицу лексем
				LT::Add(lexTable, lexEntry);
				wordNumber++;
			}

			lineNumber++;
			wordNumber = 1;
		}
	}
}