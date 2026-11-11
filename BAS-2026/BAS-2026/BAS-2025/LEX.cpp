#include "LEX.h"
#include <sstream>
#include <cctype>
#include <string>
#include <iostream>
using namespace std;

namespace LEX {
	// Вспомогательная проверка: является ли символ одиночным разделителем/оператором
	bool IsSingleDelimiter(char c) {
		switch (c) {
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		case ';': case ',':
		case ':': case '=':
		case '+': case '-':
		case '*': case '/':
		case '%': case '&':
		case '|': case '~':
			return true;
		default:
			return false;
		}
	}

	// Проверка: может ли минус перед цифрой быть частью отрицательного литерала?
	// Да, если перед минусом НЕ стояло выражение (идентификатор, число или закрывающая скобка)
	bool CanPrecedingTokenHaveUnaryMinus(const std::string& prevWord) {
		if (prevWord.empty()) {
			return true; // Самое начало строки
		}
		if (prevWord == "return" || prevWord == "case") {
			return true;
		}
		char lastChar = prevWord.back();
		// Если предыдущее слово заканчивалось на букву, цифру или ')', значит это бинарное вычитание (a - 5, 10 - 5, f() - 5)
		if (std::isalnum(static_cast<unsigned char>(lastChar)) || lastChar == '_' || lastChar == ')') {
			return false;
		}
		// Во всех остальных случаях (=, (, ,, :, +, -, return и т.д.) минус относится к числу
		return true;
	}



	void SplitIntoWords(const char* code, std::vector<std::vector<std::string>>& lines) {
		std::vector<std::string> currentLine;
		std::string currentWord;
		std::string lastWordInLine; // Запоминаем последнее слово для контекста минуса

		auto flushWord = [&]() {
			if (!currentWord.empty()) {
				currentLine.push_back(currentWord);
				lastWordInLine = currentWord;
				currentWord.clear();
			}
			};

		for (size_t i = 0; code[i] != IN_CODE_ENDS; ++i) {
			char c = code[i];

			// 1. Разделитель строк (сепаратор | из твоего In::getin)
			if (c == SEP) {
				flushWord();
				lines.push_back(currentLine);
				currentLine.clear();
				lastWordInLine.clear();
				continue;
			}

			// 2. Пробелы и табуляция вне строк
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				flushWord();
				continue;
			}

			// 3. Строковые и символьные литералы (в двойных " или одинарных ' кавычках)
			if (c == QM || c == '\'') {
				flushWord();
				char quoteType = c;
				currentWord += c;
				++i;

				while (code[i] != IN_CODE_ENDS && code[i] != SEP) {
					// Поддержка экранированных кавычек внутри строки (например, \" или \')
					if (code[i] == '\\' && code[i + 1] != IN_CODE_ENDS && code[i + 1] != SEP) {
						currentWord += code[i];
						currentWord += code[i + 1];
						i += 2;
						continue;
					}

					currentWord += code[i];
					if (code[i] == quoteType) {
						break; // Нашли закрывающую кавычку того же типа
					}
					++i;
				}

				// Если дошли до конца файла без закрывающей кавычки — откатываем i на шаг назад,
				// чтобы внешний цикл корректно увидел IN_CODE_ENDS
				if (code[i] == IN_CODE_ENDS) {
					--i;
				}
				flushWord();
				continue;
			}

			// 4. Умная обработка минуса: склеиваем с числом, если это унарный минус (например, x=-5 или (-0x1A))
			if (c == '-') {
				flushWord();
				char nextChar = code[i + 1];
				if (std::isdigit(static_cast<unsigned char>(nextChar)) && CanPrecedingTokenHaveUnaryMinus(lastWordInLine)) {
					currentWord += '-';
					// Цифры подхватятся на следующих итерациях цикла в currentWord
					continue;
				}
				else {
					// Обычный бинарный минус или минус перед переменной (-a)
					currentWord = "-";
					flushWord();
					continue;
				}
			}

			// 5. Одиночные разделители и операторы (; ( ) { } = + * / и т.д.)
			if (IsSingleDelimiter(c)) {
				flushWord();
				currentWord += c;
				flushWord();
				continue;
			}

			// 6. Обычные символы (буквы, цифры, подчёркивания — собираются в слово)
			currentWord += c;
		}

		// Сбрасываем хвост, если файл закончился без сепаратора |
		flushWord();
		if (!currentLine.empty()) {
			lines.push_back(currentLine);
		}
	}





	int HexToDecimal(const std::string& word) {
		try {
			return std::stoll(word, nullptr, 0);
		}
		catch (const std::exception& e) {
			throw ERROR_THROW(205);
		}
	}


	bool checkShortLit(const std::string& word) {
		size_t pos = 0;
		bool isNegative = false;

		if (word[0] == '-') {
			isNegative = true;
			pos = 1;
		}

		while (
			pos < word.size() &&
			word[pos] == '0') {
			pos++;
		}

		if (pos == word.size()) { return true; }

		size_t len = word.size() - pos;
		if (len < 5) { return true; }
		if (len > 5) { return false; }

		const char* limit = isNegative ? "32768" : "32767";

		for (size_t i = 0; i < 5; ++i) {
			if (word[pos + i] < limit[i]) {
				return true;
			}
			if (word[pos + i] > limit[i]) {
				return false;
			}
		}

		return true;
	}


	bool checkHexShortRange(const std::string& word) {
		size_t pos = 0;
		bool isNegative = false;

		if (word[0] == '-') {
			isNegative = true;
			pos = 1;
		}

		if (
			pos + 1 < word.size() &&
			word[pos] == '0' &&
			(word[pos + 1] == 'x' || word[pos + 1] == 'X')) {
			pos += 2;
		}

		while (pos < word.size() && word[pos] == '0') {
			pos++;
		}

		if (pos == word.size()) {
			return true;
		}

		size_t len = word.size() - pos;

		if (len < 4) {
			return true;
		}
		if (len > 4) {
			return false;
		}

		char first = word[pos];

		if (first >= '0' && first <= '7') {
			return true;
		}

		if (isNegative && first == '8') {
			return word[pos + 1] == '0' && word[pos + 2] == '0' && word[pos + 3] == '0';
		}

		return false;
	}



	int CheckLiteralByValue(const IT::IdTable& idTable, const IT::Entry& newEntry) {
		if (newEntry.idtype != IT::L) {
			return TI_NULLIDX;
		}

		for (int i = 0; i < idTable.size; i++) {
			const IT::Entry& existingEntry = idTable.table[i];

			if (existingEntry.idtype != IT::L || existingEntry.iddatatype != newEntry.iddatatype) {
				continue;
			}

			if (newEntry.iddatatype == IT::SHORT) {
				if (existingEntry.value.vshort == newEntry.value.vshort) {
					return i;
				}
			}
			else if (newEntry.iddatatype == IT::CHAR) {
				if (existingEntry.value.vchar == newEntry.value.vchar) {
					return i;
				}
			}
			else if (newEntry.iddatatype == IT::STR) {
				if (existingEntry.value.vstr->len == newEntry.value.vstr->len) {
					if (memcmp(existingEntry.value.vstr->str,
						newEntry.value.vstr->str,
						newEntry.value.vstr->len) == 0) {
						return i;
					}
				}
			}
		}

		return TI_NULLIDX;
	}


	int ProcessLiteral(const std::string& word, int lineNumber, int lexTableSize, int& literalCount, IT::IdTable& idTable) {
		IT::Entry idEntry{}; // Обязательно зануляем структуру!

		std::string litName = "L" + std::to_string(literalCount++);
		int copyLen = std::min<int>(litName.length(), ID_MAXSIZE - 1);
		memcpy(idEntry.id, litName.c_str(), copyLen);
		idEntry.id[copyLen] = IN_CODE_ENDS;
		idEntry.idtype = IT::L;
		idEntry.idxfirstLE = lexTableSize;

		// 1. Десятичный литерал (включая -00555, 05, 0)
		if (Automata::executeAutomata(Automata::NUMBER_LITERAL, word.c_str())) {
			if (!checkShortLit(word)) {
				throw ERROR_THROW_LINE(207, lineNumber);
			}
			idEntry.iddatatype = IT::SHORT;
			idEntry.value.vshort = static_cast<short>(std::stoi(word, nullptr, 10));
		}
		// 2. Шестнадцатеричный литерал
		else if (Automata::executeAutomata(Automata::HEX_NUMBER_LITERAL, word.c_str())) {
			if (!checkHexShortRange(word)) {
				throw ERROR_THROW_LINE(207, lineNumber);
			}
			idEntry.iddatatype = IT::SHORT;
			idEntry.value.vshort = static_cast<short>(HexToDecimal(word));
		}
		// 3. Символьный литерал
		else if (Automata::executeAutomata(Automata::CHAR_LITERAL, word.c_str())) {
			idEntry.iddatatype = IT::CHAR;
			if (word.size() >= 4 && word[1] == '\\') {
				switch (word[2]) {
				case 'n': idEntry.value.vchar = '\n'; break;
				case 't': idEntry.value.vchar = '\t'; break;
				case 'r': idEntry.value.vchar = '\r'; break;
				case '0': idEntry.value.vchar = '\0'; break;
				default:  idEntry.value.vchar = word[2]; break;
				}
			}
			else {
				idEntry.value.vchar = word[1];
			}
		}
		// 4. Строковый литерал
		else if (Automata::executeAutomata(Automata::STRING_LITERAL, word.c_str())) {
			idEntry.iddatatype = IT::STR;
			std::string content = word.substr(1, word.length() - 2);
			int strLen = std::min<int>(content.length(), TI_STR_MAXSIZE - 1);

			idEntry.value.vstr->len = static_cast<char>(strLen);
			memcpy(idEntry.value.vstr->str, content.c_str(), strLen);
			idEntry.value.vstr->str[strLen] = IN_CODE_ENDS;
		}

		int idIndex = CheckLiteralByValue(idTable, idEntry);
		if (idIndex == TI_NULLIDX) {
			IT::Add(idTable, idEntry);
			idIndex = idTable.size - 1;
		}
		return idIndex;
	}


	// Вспомогательная функция: определяет тип данных по слову объявления
	IT::IDDATATYPE GetDataTypeFromWord(const std::string& word) {
		if (Automata::executeAutomata(Automata::SHORT, word.c_str())) return IT::SHORT;
		if (Automata::executeAutomata(Automata::CHAR, word.c_str()))  return IT::CHAR;
		return IT::STR;
	}

	// Этап 2: Чистый проход по готовой таблице лексем LT для заполнения IT и областей видимости
	void BuildIdentifierTable(LT::LexTable& lexTable,
		const std::vector<std::string>& rawWords,
		IT::IdTable& idTable) {
		std::vector<int> scopeStack;
		scopeStack.push_back(GLOBAL_SCOPE);

		bool insideFunctionParams = false;
		int braceDepthInFunc = 0; // Счетчик вложенных { } (для switch, if, while внутри функции)

		for (int i = 0; i < lexTable.size; ++i) {
			char code = lexTable.table[i].lexema[0];
			const std::string& word = rawWords[i];

			// 1. Вход в главную функцию main
			if (code == LEX_MAIN) {
				scopeStack.push_back(MAIN_SCOPE);
				braceDepthInFunc = 0;
				continue;
			}

			// 2. Открывающая круглая скобка после имени функции -> начинаются параметры
			if (code == LEX_LEFTHESIS && i > 0 && lexTable.table[i - 1].lexema[0] == LEX_ID) {
				int prevIdIdx = lexTable.table[i - 1].idxTI;
				if (prevIdIdx != TI_NULLIDX && idTable.table[prevIdIdx].idtype == IT::F) {
					insideFunctionParams = true;
				}
				continue;
			}

			// 3. Закрывающая круглая скобка -> параметры функции закончились
			if (code == LEX_RIGHTHESIS && insideFunctionParams) {
				insideFunctionParams = false;
				continue;
			}

			// 4. Контроль фигурных скобок и областей видимости (больше не ломается на switch/if!)
			if (code == LEX_LEFTBRACE) {
				if (scopeStack.size() > 1) {
					braceDepthInFunc++;
				}
				continue;
			}
			if (code == LEX_RIGHTBRACE) {
				if (scopeStack.size() > 1) {
					braceDepthInFunc--;
					if (braceDepthInFunc <= 0) {
						scopeStack.pop_back(); // Вышли из функции или main
						braceDepthInFunc = 0;
					}
				}
				continue;
			}

			// 5. Обработка идентификаторов с полным знанием контекста
			if (code == LEX_ID) {
				if (word.length() >= ID_MAXSIZE) {
					throw ERROR_THROW_LINE(201, lexTable.table[i].sn);
				}

				char prevCode = (i > 0) ? lexTable.table[i - 1].lexema[0] : 0;
				char prevPrevCode = (i > 1) ? lexTable.table[i - 2].lexema[0] : 0;

				// Паттерн А: [тип] [function] [ID] -> Объявление функции
				if (prevCode == LEX_FUNCTION && prevPrevCode == LEX_SHORT) {
					IT::Entry idEntry{};
					strncpy_s(idEntry.id, ID_MAXSIZE, word.c_str(), word.length());
					idEntry.id[word.length()] = IN_CODE_ENDS;
					idEntry.idtype = IT::F;
					idEntry.iddatatype = GetDataTypeFromWord(rawWords[i - 2]);
					idEntry.value.vshort = TI_INT_DEFAULT;
					idEntry.idxfirstLE = i;
					idEntry.scope = GLOBAL_SCOPE;

					// Проверка на повторное объявление функции
					if (IT::IsId(idTable, idEntry.id, GLOBAL_SCOPE) != TI_NULLIDX) {
						throw ERROR_THROW_LINE(202, lexTable.table[i].sn); // Код ошибки дубликата
					}

					IT::Add(idTable, idEntry, false);
					int funcIdx = idTable.size - 1;
					lexTable.table[i].idxTI = funcIdx;

					scopeStack.push_back(funcIdx);
					braceDepthInFunc = 0;
				}
				// Паттерн Б: [тип] [ID] -> Объявление параметра или переменной
				else if (prevCode == LEX_SHORT) {
					IT::Entry idEntry{};
					strncpy_s(idEntry.id, ID_MAXSIZE, word.c_str(), word.length());
					idEntry.id[word.length()] = IN_CODE_ENDS;
					idEntry.idtype = insideFunctionParams ? IT::P : IT::V;
					idEntry.iddatatype = GetDataTypeFromWord(rawWords[i - 1]);
					idEntry.value.vshort = TI_INT_DEFAULT;
					idEntry.idxfirstLE = i;
					idEntry.scope = scopeStack.back();

					// Запрещаем две одинаковые переменные в одной области видимости
					if (IT::IsId(idTable, idEntry.id, idEntry.scope) != TI_NULLIDX) {
						throw ERROR_THROW_LINE(202, lexTable.table[i].sn);
					}

					IT::Add(idTable, idEntry, false);
					int varIdx = idTable.size - 1;
					lexTable.table[i].idxTI = varIdx;

					// Если это параметр функции — записываем его тип в метаданные функции
					if (idEntry.idtype == IT::P) {
						int funcIdx = scopeStack.back();
						int pNum = idTable.table[funcIdx].func_meta.n_params;
						idTable.table[funcIdx].func_meta.params_types[pNum] = idEntry.iddatatype;
						idTable.table[funcIdx].func_meta.n_params++;
					}
				}
				// Паттерн В: Использование уже объявленного идентификатора
				else {
					char idBuf[ID_MAXSIZE]{};
					strncpy_s(idBuf, ID_MAXSIZE, word.c_str(), word.length());

					// Сначала ищем в локальной области видимости, затем в глобальной
					int foundIdx = IT::IsId(idTable, idBuf, scopeStack.back());
					if (foundIdx == TI_NULLIDX && scopeStack.back() != GLOBAL_SCOPE) {
						foundIdx = IT::IsId(idTable, idBuf, GLOBAL_SCOPE);
					}

					if (foundIdx == TI_NULLIDX) {
						throw ERROR_THROW_LINE(208, lexTable.table[i].sn); // Необъявленный идентификатор
					}

					lexTable.table[i].idxTI = foundIdx;
				}
			}
		}
	}

	void Analyze(const char* sourceCode, LT::LexTable& lexTable, IT::IdTable& idTable) {
		std::vector<std::vector<std::string>> lines;
		SplitIntoWords(sourceCode, lines);

		std::vector<std::string> rawWords; // Храним исходные слова для 2-го прохода
		int lineNumber = 1;
		int wordNumber = 1;
		int literalCount = 1;

		// Проход 1: Токенизация и занесение литералов
		for (const auto& line : lines) {
			for (const auto& word : line) {
				char code = Automata::getLexemeCode(word.c_str());
				if (code == LEX_UNKNOWN) {
					throw ERROR_THROW_IN(200, lineNumber, wordNumber);
				}

				LT::Entry lexEntry{};
				lexEntry.lexema[0] = code;
				lexEntry.lexema[1] = IN_CODE_ENDS;
				lexEntry.sn = lineNumber;
				lexEntry.idxTI = LT_TI_NULLIDX;

				if (code == LEX_LITERAL) {
					lexEntry.idxTI = ProcessLiteral(word, lineNumber, lexTable.size, literalCount, idTable);
				}

				LT::Add(lexTable, lexEntry);
				rawWords.push_back(word);
				wordNumber++;
			}
			lineNumber++;
			wordNumber = 1;
		}

		// Проход 2: Разрешение областей видимости и заполнение таблицы идентификаторов
		BuildIdentifierTable(lexTable, rawWords, idTable);
	}


}