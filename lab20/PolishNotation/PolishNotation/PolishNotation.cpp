#include "stdafx.h"
#include "PolishNotation.h"
#include <stack>
#include <queue>
#include <iostream>


namespace PN {
    //================================================================
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
    //================================================================



    bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable) {
        // 1. Проверка входных данных
        if (lextable_pos >= lextable.size) return false;

        // 2. Найти конец выражения (до ';')
        int start = lextable_pos;
        int end = start;
        while (end < lextable.size && lextable.table[end].lexema[0] != ';') {
            end++;
        }

        if (end <= start) return false; // пустое выражение

        // 3. Преобразование инфикс → постфикс
        std::stack<LT::Entry> op_stack;
        std::queue<LT::Entry> output;

        for (int i = start; i < end; i++) {
            LT::Entry token = lextable.table[i];
            char lex = token.lexema[0];

            // 3.1 Операнд → сразу в выход
            if (IsOperand(lex)) {
                output.push(token);
            }
            // 3.2 '(' → в стек
            else if (lex == '(') {
                op_stack.push(token);
            }
            // 3.3 ')' → выталкиваем до '('
            else if (lex == ')') {
                while (!op_stack.empty() && op_stack.top().lexema[0] != '(') {
                    output.push(op_stack.top());
                    op_stack.pop();
                }
                if (!op_stack.empty()) op_stack.pop(); // удаляем '('
            }
            // 3.4 ОПЕРАТОР (+, -, *, /, =)
            else if (IsOperator(lex)) {
                // Проверяем унарный минус
                bool is_unary = (lex == '-' && IsUnaryMinus(i, lextable));
                int priority = GetPriority(lex);

                if (is_unary) {
                    // Унарный минус: сразу в стек с высоким приоритетом
                    op_stack.push(token);
                }
                else {
                    // Бинарный оператор: выталкиваем операторы с >= приоритетом
                    while (!op_stack.empty()) {
                        char top_lex = op_stack.top().lexema[0];
                        if (top_lex == '(') break;

                        int top_priority = GetPriority(top_lex);
                        if (top_priority < priority) break;

                        output.push(op_stack.top());
                        op_stack.pop();
                    }
                    op_stack.push(token);
                }
            }
            // 3.5 ',' → игнорируем (не нужен в ПОЛИЗ)
            else if (lex == ',') {
                // Ничего не делаем
            }
            // 3.6 Неизвестная лексема
            else {
                std::cerr << "Неизвестная лексема: " << lex << std::endl;
                return false;
            }
        }

        // 4. Выталкиваем оставшиеся операторы
        while (!op_stack.empty()) {
            if (op_stack.top().lexema[0] == '(') {
                std::cerr << "Несбалансированные скобки" << std::endl;
                return false;
            }
            output.push(op_stack.top());
            op_stack.pop();
        }

        // 5. Записать ПОЛИЗ обратно в таблицу
        int write_pos = start;
        while (!output.empty() && write_pos < end) {
            lextable.table[write_pos++] = output.front();
            output.pop();
        }

        // 6. Заполнить остаток символом-заполнителем
        const char FILLER = '~';
        while (write_pos < end) {
            lextable.table[write_pos].lexema[0] = FILLER;
            lextable.table[write_pos].idxTI = LT_TI_NULLIDX;
            write_pos++;
        }

        // 7. Обновить idxfirstLE
        for (int i = start; i < end; i++) {
            char lex = lextable.table[i].lexema[0];
            if (lex == 'i' || lex == 'l') {
                int idxTI = lextable.table[i].idxTI;
                if (idxTI >= 0 && idxTI < idtable.size) {
                    idtable.table[idxTI].idxfirstLE = i;
                }
            }
        }

        // 8. Вывести результат
        std::cout << "ПОЛИЗ: ";
        for (int i = start; i < end; i++) {
            if (lextable.table[i].lexema[0] != FILLER) {
                std::cout << lextable.table[i].lexema[0] << " ";
            }
        }
        std::cout << std::endl;

        std::cout << "ПОЛИЗ (расшифрованная): ";
        for (int i = start; i < end; i++) {
            if (lextable.table[i].lexema[0] != FILLER) {
                PrintLexemeWithDecode(lextable.table[i], idtable);
                std::cout << " ";
            }
        }
        std::cout << std::endl;

        return true;
    }


    bool ConvertAllExpressions(LT::LexTable& lextable, IT::IdTable& idtable) {
        std::cout << "=== ПРЕОБРАЗОВАНИЕ ВСЕХ ВЫРАЖЕНИЙ ===" << std::endl;

        bool success = true;

        for (int i = 0; i < lextable.size; i++) {
            if (lextable.table[i].lexema[0] == '=') {
                int expr_start = i + 1;

                std::cout << "\nВыражение (поз. " << expr_start << "): ";

                // Покажем исходное выражение
                for (int j = expr_start; j < lextable.size; j++) {
                    if (lextable.table[j].lexema[0] == ';') break;
                    std::cout << lextable.table[j].lexema[0] << " ";
                }
                std::cout << std::endl;

                // Преобразуем
                if (!PolishNotation(expr_start, lextable, idtable)) {
                    success = false;
                    std::cout << "Ошибка!" << std::endl;
                }
            }
        }

        return success;
    }
}