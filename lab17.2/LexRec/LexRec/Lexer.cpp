#include "Lexer.h"
#include "Automata.h"
#include <sstream>

namespace Lexer {

    Result Analyze(In::IN input, Log::LOG log) {
        Result result;

        std::string text((const char*)input.text);
        std::stringstream ss(text);
        std::string line;
        int lineNumber = 1;

        try {
            while (std::getline(ss, line, '|')) {
                if (line.empty()) continue;

                std::stringstream lineStream(line);
                std::string token;
                int position = 1;

                while (lineStream >> token) {
                    // 1. Определяем тип лексемы с помощью автоматов
                    char lexCode = Automata::getLexemeCode(token.c_str());

                    // 2. Создаем запись в таблице лексем
                    LT::Entry ltEntry;
                    ltEntry.lexema[0] = lexCode;
                    ltEntry.sn = lineNumber;
                    ltEntry.idxTI = LT_TI_NULLIDX;

                    // 3. Для идентификаторов и литералов добавляем в ТИ
                    if (lexCode == LEX_ID || lexCode == LEX_LITERAL) {
                        // Проверяем, есть ли уже в ТИ
                        int tiIndex = IT::IsId(result.idTable, (char*)token.c_str());
                        if (tiIndex == TI_NULLIDX) {
                            // Добавляем новый элемент в ТИ
                            IT::Entry itEntry;
                            strncpy_s(itEntry.id, ID_MAXSIZE, token.c_str(), _TRUNCATE);
                            itEntry.idxfirstLE = result.lexTable.size;

                            // ПРОСТОЙ подход: идентификатор или литерал
                            if (lexCode == LEX_ID) {
                                itEntry.idtype = IT::V; // все идентификаторы - переменные
                                itEntry.iddatatype = IT::INT; // по умолчанию integer
                            }
                            else {
                                itEntry.idtype = IT::L; // литералы
                                itEntry.iddatatype = (token[0] == '\'') ? IT::STR : IT::INT;
                            }

                            IT::Add(result.idTable, itEntry);
                            tiIndex = result.idTable.size - 1;
                        }
                        ltEntry.idxTI = tiIndex;
                    }

                    // 4. Добавляем в таблицу лексем
                    LT::Add(result.lexTable, ltEntry);

                    position += (int)token.length() + 1;
                }

                lineNumber++;
            }
        }
        catch (Error::ERROR error) {
            Log::WriteError(log, error);
            throw;
        }

        return result;
    }
}