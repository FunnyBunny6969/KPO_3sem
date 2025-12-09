#include "PolishNotation.h"
#include <vector>
#include <stack>
#include <iostream>

namespace PN {

    // Глобальная переменная для отслеживания уровня вложенности
    static int g_nestingLevel = 0;

    namespace {

        int GetPriority(char op) {
            switch (op) {
            case '*': case '/': return 7;
            case '+': case '-': return 6;
            case '=':           return 1;  // самый низкий
            default:            return 0;
            }
        }

        bool IsOperator(char op) {
            return op == LEX_PLUS || op == '-' || op == '*' || op == '/' || op == '=';
        }

        bool IsOperand(char lex) {
            return lex == 'i' || lex == 'l' || lex == 't' || lex == 'n';
        }

        bool IsUnaryMinus(int pos, const LT::LexTable& lextable) {
            if (lextable.table[pos].lexema[0] != '-') return false;

            // Начало выражения?
            if (pos == 0) return true;

            // После '=', '(', ',' или другого оператора?
            char prev = lextable.table[pos - 1].lexema[0];
            return (prev == '=' || prev == '(' || prev == ',' ||
                prev == '+' || prev == '-' || prev == '*' || prev == '/');
        }


        // Вспомогательная функция: печать лексемы с расшифровкой
        void PrintLexemeWithDecode(const LT::Entry& entry, const IT::IdTable& idtable) {
            char lex = entry.lexema[0];

            if (lex == 'i' || lex == 'l') {  // идентификатор или литерал
                if (entry.idxTI != LT_TI_NULLIDX && entry.idxTI < idtable.size) {
                    IT::Entry id_entry = idtable.table[entry.idxTI];

                    if (lex == 'i') {
                        std::cout << "{" << id_entry.id << "}";  // имя идентификатора
                    }
                    else if (lex == 'l') {
                        // Литерал: покажем значение
                        if (id_entry.iddatatype == IT::INT) {
                            std::cout << "[" << id_entry.value.vint << "]";  // число
                        }
                        else if (id_entry.iddatatype == IT::STR) {
                            std::cout << "['";
                            for (int i = 0; i < (int)id_entry.value.vstr->len; i++) {
                                std::cout << id_entry.value.vstr->str[i];
                            }
                            std::cout << "']";
                        }
                    }
                }
                else {
                    std::cout << lex << "?";  // неизвестно
                }
            }
            else if (lex == '~') {
                std::cout << "~";  // заполнитель
            }
            else {
                std::cout << lex;  // операторы, скобки и т.д.
            }
        }
    }


    ScanResult Scanner(const LT::LexTable& lextable, int start, int end) {
        ScanResult result = { -1, -1, -1, 0 };

        for (int i = start; i < end; i++) {
            char lex = lextable.table[i].lexema[0];

            // 1. Присваивание (i = выражение)
            if (lex == LEX_ID && i + 1 < end &&
                lextable.table[i + 1].lexema[0] == LEX_EQUALS) {
                result.type = 1;
                result.start = i + 2; // после "i ="

                // Ищем конец (до ';' или конца диапазона)
                for (int j = result.start; j < end; j++) {
                    if (lextable.table[j].lexema[0] == LEX_SEMICOLON) {
                        result.end = j;
                        return result;
                    }
                }
                result.end = end;
                return result;
            }

            // 2. Вызов функции (ID '(' ... ')')
            if (lex == LEX_ID && i + 1 < end &&
                lextable.table[i + 1].lexema[0] == LEX_LEFTHESIS) {
                result.type = 2;
                result.start = i;
                result.functionName = lex;

                // Ищем закрывающую скобку
                int depth = 1;
                for (int j = i + 2; j < end; j++) {
                    char l = lextable.table[j].lexema[0];
                    if (l == LEX_LEFTHESIS) depth++;
                    else if (l == LEX_RIGHTHESIS) {
                        depth--;
                        if (depth == 0) {
                            result.end = j + 1; // включая ')'
                            return result;
                        }
                    }
                }
                result.end = end;
                return result;
            }

            // 3. Арифметическое выражение в скобках
            if (lex == LEX_LEFTHESIS) {
                result.type = 3;
                result.start = i + 1; // после '('

                // Ищем парную закрывающую скобку
                int depth = 1;
                for (int j = i + 1; j < end; j++) {
                    char l = lextable.table[j].lexema[0];
                    if (l == LEX_LEFTHESIS) depth++;
                    else if (l == LEX_RIGHTHESIS) {
                        depth--;
                        if (depth == 0) {
                            result.end = j; // не включая ')'
                            return result;
                        }
                    }
                }
                result.end = end;
                return result;
            }
        }

        return result; // type = -1 значит ничего не найдено
    }

    // ================== РЕКУРСИВНЫЙ POLISH NOTATION ==================
    bool PolishNotationRecursive(LT::LexTable& lextable, IT::IdTable& idtable,
        int start, int end, bool isTopLevel = false) {

        g_nestingLevel++;

        // Определяем символ конца для этого уровня
        char stopChar = (g_nestingLevel == 1 || isTopLevel) ?
            LEX_SEMICOLON : LEX_RIGHTHESIS;

        // Находим реальный конец выражения
        int realEnd = start;
        while (realEnd < lextable.size && realEnd < end) {
            char lex = lextable.table[realEnd].lexema[0];
            if (lex == stopChar || lex == LEX_COMMA) break;
            realEnd++;
        }

        if (realEnd <= start) {
            g_nestingLevel--;
            return false;
        }

        // Сканируем и обрабатываем все вложенные выражения
        bool modified = true;
        while (modified) {
            modified = false;
            ScanResult scan = Scanner(lextable, start, realEnd);

            if (scan.type != -1) {
                // Обрабатываем найденное выражение
                if (scan.type == 2) { // Вызов функции
                    // Обрабатываем аргументы функции
                    int argStart = scan.start + 2; // после ID и '('
                    int argEnd = scan.end - 1;     // перед ')'

                    // Обрабатываем каждый аргумент
                    int pos = argStart;
                    int argCount = 0;

                    while (pos < argEnd) {
                        // Находим границы аргумента
                        int argStartPos = pos;
                        int argEndPos = argStartPos;
                        int depth = 0;

                        while (argEndPos < argEnd) {
                            char l = lextable.table[argEndPos].lexema[0];
                            if (l == LEX_LEFTHESIS) depth++;
                            else if (l == LEX_RIGHTHESIS) depth--;
                            else if (l == LEX_COMMA && depth == 0) break;
                            argEndPos++;
                        }

                        if (argEndPos > argStartPos) {
                            // Рекурсивно обрабатываем аргумент
                            PolishNotationRecursive(lextable, idtable,
                                argStartPos, argEndPos, false);
                            argCount++;
                        }

                        pos = argEndPos;
                        if (pos < argEnd && lextable.table[pos].lexema[0] == LEX_COMMA) {
                            pos++;
                        }
                    }

                    // Преобразуем сам вызов функции в польскую запись
                    ConvertFunctionCall(lextable, idtable, scan.start, scan.end, argCount);
                }
                else { // Присваивание или скобочное выражение
                    PolishNotationRecursive(lextable, idtable,
                        scan.start, scan.end, scan.type == 1);
                }

                modified = true;
            }
        }

        // Теперь преобразуем основное выражение (уже без вложенных)
        ConvertSimpleExpression(lextable, idtable, start, realEnd);

        g_nestingLevel--;
        return true;
    }

    // ================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ==================

    void ConvertFunctionCall(LT::LexTable& lextable, IT::IdTable& idtable,
        int start, int end, int argCount) {
        // Сохраняем имя функции
        LT::Entry funcName = lextable.table[start];

        // Собираем аргументы
        std::vector<LT::Entry> args;
        for (int i = start + 2; i < end - 1; i++) {
            if (lextable.table[i].lexema[0] != LEX_COMMA &&
                lextable.table[i].lexema[0] != LEX_LEFTHESIS &&
                lextable.table[i].lexema[0] != LEX_RIGHTHESIS) {
                args.push_back(lextable.table[i]);
            }
        }

        // Записываем в польской записи: аргументы, имя, @N
        int writePos = start;

        // Аргументы
        for (const auto& arg : args) {
            if (writePos < end) {
                lextable.table[writePos++] = arg;
            }
        }

        // Имя функции
        if (writePos < end) {
            lextable.table[writePos++] = funcName;
        }

        // Маркер вызова функции с N аргументами
        if (writePos < end) {
            lextable.table[writePos].lexema[0] = '@';
            lextable.table[writePos].idxTI = argCount;
            writePos++;
        }

        // Заполнитель для остатка
        while (writePos < end) {
            lextable.table[writePos].lexema[0] = '~';
            lextable.table[writePos].idxTI = LT_TI_NULLIDX;
            writePos++;
        }
    }

    void ConvertSimpleExpression(LT::LexTable& lextable, IT::IdTable& idtable,
        int start, int end) {
        // Классический Shunting-yard для арифметики
        std::stack<LT::Entry> opStack;
        std::vector<LT::Entry> output;

        for (int i = start; i < end; i++) {
            LT::Entry token = lextable.table[i];
            char lex = token.lexema[0];

            // Пропускаем уже обработанные маркеры
            if (lex == '@' || lex == '~') continue;

            if (IsOperand(lex)) {
                output.push_back(token);
            }
            else if (lex == LEX_LEFTHESIS) {
                opStack.push(token);
            }
            else if (lex == LEX_RIGHTHESIS) {
                while (!opStack.empty() && opStack.top().lexema[0] != LEX_LEFTHESIS) {
                    output.push_back(opStack.top());
                    opStack.pop();
                }
                if (!opStack.empty()) opStack.pop();
            }
            else if (IsOperator(lex)) {
                int priority = GetPriority(lex);
                while (!opStack.empty() && opStack.top().lexema[0] != LEX_LEFTHESIS &&
                    GetPriority(opStack.top().lexema[0]) >= priority) {
                    output.push_back(opStack.top());
                    opStack.pop();
                }
                opStack.push(token);
            }
        }

        while (!opStack.empty()) {
            output.push_back(opStack.top());
            opStack.pop();
        }

        // Записать результат
        int writePos = start;
        for (const auto& entry : output) {
            if (writePos < end) {
                lextable.table[writePos++] = entry;
            }
        }

        while (writePos < end) {
            lextable.table[writePos].lexema[0] = '~';
            lextable.table[writePos].idxTI = LT_TI_NULLIDX;
            writePos++;
        }
    }

    // ================== ИНТЕРФЕЙСНЫЕ ФУНКЦИИ ==================

    bool PolishNotation(int startPos, LT::LexTable& lextable, IT::IdTable& idtable) {
        g_nestingLevel = 0;
        return PolishNotationRecursive(lextable, idtable, startPos, lextable.size, true);
    }

    bool ConvertAllExpressions(LT::LexTable& lextable, IT::IdTable& idtable) {
        std::cout << "=== АВТОМАТИЧЕСКОЕ ПРЕОБРАЗОВАНИЕ ===" << std::endl;

        // Ищем все выражения и обрабатываем их
        for (int i = 0; i < lextable.size; i++) {
            char lex = lextable.table[i].lexema[0];

            if (lex == LEX_EQUALS || lex == LEX_RETURN || lex == LEX_PRINT) {
                int exprStart = i + 1;

                // Находим конец выражения
                int exprEnd = exprStart;
                while (exprEnd < lextable.size &&
                    lextable.table[exprEnd].lexema[0] != LEX_SEMICOLON) {
                    exprEnd++;
                }

                if (exprEnd > exprStart) {
                    std::cout << "Обработка выражения на позиции " << exprStart << std::endl;
                    PolishNotation(exprStart, lextable, idtable);
                }
            }
        }

        return true;
    }
}