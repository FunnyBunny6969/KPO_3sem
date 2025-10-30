#include "Lexer.h"
#include "Automata.h"
#include <cstring>
#include <iostream>

namespace Lexer {

    RESULT Analyze(In::IN input, Log::LOG log) {
        RESULT result;
        result.lextable = LT::Create(LT_MAXSIZE);
        result.idtable = IT::Create(TI_MAXSIZE);

        try {
            Internal::ProcessText((const char*)input.text, result, log);
        }
        catch (...) {
            LT::Delete(result.lextable);
            IT::Delete(result.idtable);
            throw;
        }

        return result;
    }

    namespace Internal {

        void ProcessText(const char* text, RESULT& result, Log::LOG log) {
            int line = 1;
            int position = 1;
            const char* current = text;
            char token[256];

            while (*current) {
                // Пропускаем пробелы
                if (*current == ' ') {
                    current++;
                    position++;
                    continue;
                }

                // Обрабатываем разделитель строк
                if (*current == '|') {
                    line++;
                    position = 1;
                    current++;
                    continue;
                }

                // Выделяем лексему
                ExtractToken(current, token);

                if (strlen(token) > 0) {
                    // Распознаем лексему
                    if (!RecognizeToken(token, line, position, result, log)) {
                        throw ERROR_THROW_IN(203, line, position); // Неизвестная лексема
                    }

                    // Двигаем позицию
                    position += strlen(token);
                    current += strlen(token);
                }
                else {
                    current++;
                }
            }
        }

        void ExtractToken(const char*& current, char* token) {
            int i = 0;
            const char* start = current;

            // Копируем символы до пробела, | или конца строки
            while (*current && *current != ' ' && *current != '|' && *current != '\0') {
                token[i++] = *current++;
            }
            token[i] = '\0';
        }

        bool RecognizeToken(const char* token, int line, int pos, RESULT& result, Log::LOG log) {
            FST::FST fst;

            // Проверяем ключевые слова
            fst = Automata::INTEGER;
            fst.string = (char*)token;
            if (FST::execute(fst)) {
                AddToTables(result, 't', "integer", line, pos, log);
                return true;
            }

            fst = Automata::FUNCTION;
            fst.string = (char*)token;
            if (FST::execute(fst)) {
                AddToTables(result, 'f', "function", line, pos, log);
                return true;
            }

            // Проверяем идентификаторы
            fst = Automata::IDENTIFIER;
            fst.string = (char*)token;
            if (FST::execute(fst)) {
                AddIdentifier(result, token, line, pos, log);
                return true;
            }

            // Проверяем одиночные символы
            if (strlen(token) == 1) {
                char ch = token[0];
                switch (ch) {
                case ';': AddToTables(result, ';', ";", line, pos, log); return true;
                case ',': AddToTables(result, ',', ",", line, pos, log); return true;
                case '(': AddToTables(result, '(', "(", line, pos, log); return true;
                case ')': AddToTables(result, ')', ")", line, pos, log); return true;
                case '{': AddToTables(result, '{', "{", line, pos, log); return true;
                case '}': AddToTables(result, '}', "}", line, pos, log); return true;
                case '=': AddToTables(result, 'v', "=", line, pos, log); return true;
                case '+': AddToTables(result, 'v', "+", line, pos, log); return true;
                case '-': AddToTables(result, 'v', "-", line, pos, log); return true;
                case '*': AddToTables(result, 'v', "*", line, pos, log); return true;
                case '/': AddToTables(result, 'v', "/", line, pos, log); return true;
                }
            }

            return false;
        }

        void AddToTables(RESULT& result, char lexType, const char* value, int line, int pos, Log::LOG log) {
            LT::Entry ltEntry;
            ltEntry.lexema[0] = lexType;
            ltEntry.lexema[1] = '\0';
            ltEntry.sn = line;

            // Для идентификаторов устанавливаем ссылку на IT
            if (lexType == 'i') {
                // Находим индекс в IT
                char id[ID_MAXSIZE];
                strcpy_s(id, value);
                int tiIndex = IT::IsId(result.idtable, id);
                ltEntry.idxTI = (tiIndex != TI_NULLIDX) ? tiIndex : LT_TI_NULLIDX;
            }
            else {
                ltEntry.idxTI = LT_TI_NULLIDX;
            }

            LT::Add(result.lextable, ltEntry);
        }

        void AddIdentifier(RESULT& result, const char* id, int line, int pos, Log::LOG log) {
            // Проверяем, есть ли уже такой идентификатор
            char idCopy[ID_MAXSIZE];
            strcpy_s(idCopy, id);
            int existingIndex = IT::IsId(result.idtable, idCopy);

            if (existingIndex == TI_NULLIDX) {
                // Добавляем новый идентификатор
                IT::Entry itEntry;
                strcpy_s(itEntry.id, id);
                itEntry.idxfirstLE = result.lextable.size; // Ссылка на следующую лексему
                itEntry.iddatatype = IT::INT; // По умолчанию integer
                itEntry.idtype = IT::V;       // По умолчанию переменная
                itEntry.value.vint = TI_INT_DEFAULT;

                IT::Add(result.idtable, itEntry);
                existingIndex = result.idtable.size - 1;
            }

            // Добавляем лексему идентификатора
            AddToTables(result, 'i', id, line, pos, log);
        }
    }
}