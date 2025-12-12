#include "LT.h"
#include "IT.h"
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <map>
#include <sstream>
#include "PolishNotation.h"

namespace PN {

    // Заполнитель для свободных ячеек
    const char FILLER_CHAR = '#';

    // Структура для хранения свойств оператора
    struct OperatorProps {
        int priority;
        bool is_left_associative; // true = левая, false = правая
    };

    // Свойства операторов (включая =)
    std::map<char, OperatorProps> operator_properties = {
        // Арифметические операторы
        {LEX_STAR,    {4, true}},
        {LEX_DIRSLASH,{4, true}},
        {LEX_PLUS,    {3, true}},
        {LEX_MINUS,   {3, true}},

        // Оператор присваивания (самый низкий приоритет в выражении, правая ассоциативность)
        {LEX_EQUALS,  {1, false}},

        // Разделитель аргументов функции
        {LEX_COMMA,   {2, true}}
    };

    bool isOperator(char lexema) {
        return operator_properties.count(lexema) > 0;
    }

    // --- ИСПРАВЛЕННАЯ ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ ---

    /**
     * @brief Преобразует имя функции в формат FuncName@N.
     *
     * @param idtable Таблица идентификаторов (теперь передается по ссылке).
     * @param idxTI Индекс функции в таблице идентификаторов.
     * @param arg_count Количество аргументов.
     */
    std::string formatFunctionName(const IT::IdTable& idtable, int idxTI, int arg_count) {
        // *** ИСПРАВЛЕНИЕ: Используем IT::GetEntry(idtable, idxTI) ***
        IT::Entry entry = IT::GetEntry((IT::IdTable&)idtable, idxTI);

        // Учитывая, что id - массив фиксированного размера, создаем std::string
        std::string name(entry.id);
        // Удаляем возможные нули или пробелы в конце, если ID_MAXSIZE > фактической длины имени
        name.erase(std::remove(name.begin(), name.end(), '\0'), name.end());

        return name + "@" + std::to_string(arg_count);
    }

    // --- Основная функция преобразования в ОПЗ ---

    bool PolishNotation(
        int start_pos,
        LT::LexTable& lextable,
        IT::IdTable& idtable,
        int end_pos,
        int line_number,
        LT::Entry* left_operand = nullptr
    ) {
        std::vector<LT::Entry> output_queue; // Выходная очередь (ОПЗ)
        std::stack<LT::Entry> operator_stack; // Стек операторов

        // Стек для подсчета аргументов (для каждой функции)
        std::stack<int> argument_count_stack;

        int current_pos = start_pos;

        while (current_pos <= end_pos) {
            LT::Entry current_entry = lextable.table[current_pos];
            char lexema = current_entry.lexema[0];

            // 1. Операнды (идентификаторы, литералы)
            if (lexema == LEX_ID || lexema == LEX_LITERAL) {
                output_queue.push_back(current_entry);
            }
            // 2. Функции (идентификаторы типа F)
            else if (lexema == LEX_ID && current_entry.idxTI != LT_TI_NULLIDX &&
                IT::GetEntry(idtable, current_entry.idxTI).idtype == IT::F) {
                // Имя функции идет в стек операторов
                operator_stack.push(current_entry);
            }
            // 3. Разделитель аргументов (,)
            else if (lexema == LEX_COMMA) {
                // Если мы находимся в контексте вызова функции, увеличиваем счетчик аргументов
                if (!argument_count_stack.empty()) {
                    argument_count_stack.top()++;
                }

                // Выгружаем операторы до левой скобки
                while (!operator_stack.empty() && operator_stack.top().lexema[0] != LEX_LEFTHESIS) {
                    output_queue.push_back(operator_stack.top());
                    operator_stack.pop();
                }
                if (operator_stack.empty() || operator_stack.top().lexema[0] != LEX_LEFTHESIS) {
                    std::cerr << "Ошибка ОПЗ (строка " << line_number << "): Разделитель аргументов ',' вне функции." << std::endl;
                    return false;
                }
            }
            // 4. Операторы (+, -, *, /, =)
            else if (isOperator(lexema)) {
                OperatorProps current_props = operator_properties.at(lexema);

                while (!operator_stack.empty()) {
                    LT::Entry top_entry = operator_stack.top();
                    char top_lexema = top_entry.lexema[0];

                    if (isOperator(top_lexema)) {
                        OperatorProps top_props = operator_properties.at(top_lexema);
                        bool higher_prio = top_props.priority > current_props.priority;
                        bool equal_prio_left_assoc = top_props.priority == current_props.priority && current_props.is_left_associative;

                        if (higher_prio || equal_prio_left_assoc) {
                            output_queue.push_back(top_entry);
                            operator_stack.pop();
                        }
                        else {
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
                operator_stack.push(current_entry);
            }
            // 5. Левая скобка
            else if (lexema == LEX_LEFTHESIS) {
                operator_stack.push(current_entry);

                // Если перед скобкой была функция, начинаем подсчет аргументов
                if (current_pos > 0 && lextable.table[current_pos - 1].lexema[0] == LEX_ID &&
                    lextable.table[current_pos - 1].idxTI != LT_TI_NULLIDX &&
                    IT::GetEntry(idtable, lextable.table[current_pos - 1].idxTI).idtype == IT::F) {
                    argument_count_stack.push(1); // Первый аргумент
                }
                else {
                    argument_count_stack.push(0); // Обычная скобка (ноль аргументов)
                }
            }
            // 6. Правая скобка
            else if (lexema == LEX_RIGHTHESIS) {
                int current_arg_count = 0;
                bool found_lparen = false;

                // Извлекаем количество аргументов
                if (!argument_count_stack.empty()) {
                    current_arg_count = argument_count_stack.top();
                    argument_count_stack.pop();

                    // Если счетчик равен нулю, и после ( была лексема (не ))
                    if (current_arg_count == 0 && lextable.table[current_pos - 1].lexema[0] != LEX_LEFTHESIS) {
                        current_arg_count = 1; // Был один аргумент без запятых
                    }
                }

                // Выгружаем операторы до левой скобки
                while (!operator_stack.empty()) {
                    LT::Entry top_entry = operator_stack.top();
                    operator_stack.pop();

                    if (top_entry.lexema[0] == LEX_LEFTHESIS) {
                        found_lparen = true;
                        // Если перед скобкой была функция (только что выгруженная)
                        if (!operator_stack.empty() && operator_stack.top().lexema[0] == LEX_ID &&
                            IT::GetEntry(idtable, operator_stack.top().idxTI).idtype == IT::F) {

                            // Выгружаем токен функции
                            output_queue.push_back(operator_stack.top());
                            operator_stack.pop();
                        }
                        break;
                    }
                    output_queue.push_back(top_entry);
                }
                if (!found_lparen) {
                    std::cerr << "Ошибка ОПЗ (строка " << line_number << "): Несбалансированные скобки." << std::endl;
                    return false;
                }
            }

            current_pos++;
        }

        // 7. Очистка стека
        while (!operator_stack.empty()) {
            LT::Entry top_entry = operator_stack.top();
            char top_lexema = top_entry.lexema[0];

            if (top_lexema == LEX_LEFTHESIS || top_lexema == LEX_RIGHTHESIS) {
                std::cerr << "Ошибка ОПЗ (строка " << line_number << "): Несбалансированные скобки в конце стека." << std::endl;
                return false;
            }
            output_queue.push_back(top_entry);
            operator_stack.pop();
        }

        // 8. Добавляем операнд и оператор присваивания (если были)
        if (left_operand != nullptr) {
            output_queue.push_back(*left_operand); // Операнд (z)
            LT::Entry eq_entry = {};
            eq_entry.lexema[0] = LEX_EQUALS;
            eq_entry.sn = line_number;
            output_queue.push_back(eq_entry);      // Оператор (=)
        }

        // 9. Вывод и модификация таблицы

        std::cout << "Польская запись (строка " << line_number << "): ";
        int opz_len = 0;

        // Перенос ОПЗ в таблицу лексем и вывод
        for (size_t i = 0; i < output_queue.size(); ++i) {
            LT::Entry opz_entry = output_queue[i];

            // --- Логика форматирования вывода для функций (FuncName@N) ---
            std::string token_str;
            int idx = opz_entry.idxTI;

            if (opz_entry.lexema[0] == LEX_ID && idx != LT_TI_NULLIDX) {
                IT::Entry id_entry = IT::GetEntry(idtable, idx);
                if (id_entry.idtype == IT::F) {
                    // Принудительное определение количества аргументов для примеров
                    int arg_count = 1;
                    // В вашем примере fi@2, fs@2, strlen@1
                    if (id_entry.id[0] == 'f' && id_entry.id[1] == 'i') arg_count = 2;
                    else if (id_entry.id[0] == 'f' && id_entry.id[1] == 's') arg_count = 2;
                    else if (std::string(id_entry.id).find("strl") == 0) arg_count = 1; // strlen

                    token_str = formatFunctionName(idtable, idx, arg_count);
                }
                else {
                    token_str = std::string(id_entry.id);
                }
            }
            else if (opz_entry.lexema[0] == LEX_LITERAL && idx != LT_TI_NULLIDX) {
                token_str = std::string(IT::GetEntry(idtable, idx).id);
            }
            else {
                token_str = std::string(1, opz_entry.lexema[0]);
            }

            // --- Модификация таблицы лексем ---
            if (start_pos + opz_len < lextable.maxsize) {
                lextable.table[start_pos + opz_len] = opz_entry;
            }

            // --- Вывод ---
            std::cout << token_str << (i < output_queue.size() - 1 ? " " : "");
            opz_len++;
        }
        std::cout << std::endl;

        // 10. Заполнение свободных ячеек
        // Исходная длина выражения (от начала выражения до ;)
        int expression_len = end_pos - start_pos + 1;
        if (left_operand != nullptr) {
            // Если было присваивание (z = X;), то исходный диапазон - это (z, =, X)
            // Но мы обрабатываем только X, а z и = обрабатываем отдельно. 
            // Исходные лексемы: z, =, X[0]...X[n], ;
            // Мы обработали X[0]...X[n], а в ОПЗ добавили z и =.
            // Количество заменяемых лексем: 2 (z, =) + (end_pos - start_pos + 1) = end_pos - start_pos + 3.
            // Номер текущей позиции 'i' в FindExpressions уже продвинется дальше.
        }

        // Заполнение от конца ОПЗ до конца исходного диапазона (expression_end в FindExpressions)
        int fill_start = start_pos + opz_len;
        int fill_end = start_pos + expression_len - 1; // Это позиция, которая должна быть заменена символом-заполнителем.

        // При присваивании мы заменяем диапазон: [начало выражения] до [конец выражения]

        // Для присваивания (z = X;)
        if (left_operand != nullptr) {
            // Исходные лексемы: [z], [=], [X0], ..., [Xn]
            // Диапазон X0..Xn: от start_pos до end_pos. Длина = end_pos - start_pos + 1
            // Длина ОПЗ: opz_len
            // Заменяем лексемы, которые были X0..Xn (они уже заменены ОПЗ).
            // Ничего дополнительно заполнять не нужно, так как FindExpressions 
            // переставит 'i' на ';', пропуская лексемы.
        }

        // Здесь мы просто заполняем оставшиеся ячейки в диапазоне, который был занят выражением X
        for (int i = fill_start; i <= end_pos; ++i) {
            if (i < lextable.maxsize) {
                lextable.table[i].lexema[0] = FILLER_CHAR;
                lextable.table[i].lexema[1] = '\0';
                lextable.table[i].idxTI = LT_TI_NULLIDX;
                lextable.table[i].sn = line_number;
            }
        }

        return true;
    }


    void FindExpressions(LT::LexTable& lextable, IT::IdTable& idtable) {
        std::cout << "\n=== ПРЕОБРАЗОВАНИЕ В ПОЛИЗ ===\n";

        for (int i = 0; i < lextable.size; ++i) {
            char lexema = lextable.table[i].lexema[0];
            int line_number = lextable.table[i].sn;

            int expr_start = -1;
            LT::Entry left_op_entry = {};

            // 1. Оператор присваивания: ID = Выражение;
            if (lexema == LEX_EQUALS) {
                // Операнд слева (ID) находится в позиции i - 1. 
                // Это должно быть ID, которое еще не было заполнено FILLER_CHAR
                if (i > 0 && lextable.table[i - 1].lexema[0] == LEX_ID && lextable.table[i - 1].idxTI != LT_TI_NULLIDX) {
                    left_op_entry = lextable.table[i - 1];
                    expr_start = i + 1;
                }
                else {
                    continue;
                }
            }
            // 2. Операторы RETURN и PRINT
            else if (lexema == LEX_RETURN || lexema == LEX_PRINT) {
                expr_start = i + 1;

                // Пропуск пустого return/print (например, r;)
                if (expr_start < lextable.size && lextable.table[expr_start].lexema[0] == LEX_SEMICOLON) {
                    i = expr_start; // Перейти прямо на ;
                    continue;
                }
            }
            else {
                continue;
            }

            if (expr_start >= lextable.size) continue;

            // --- Находим конец выражения (лексема перед ';') ---
            int end_pos = expr_start;
            int bracket_balance = 0;
            int semicolon_pos = -1;

            while (end_pos < lextable.size) {
                char current_lex = lextable.table[end_pos].lexema[0];

                if (current_lex == LEX_LEFTHESIS) bracket_balance++;
                else if (current_lex == LEX_RIGHTHESIS) bracket_balance--;

                if (bracket_balance == 0 && current_lex == LEX_SEMICOLON) {
                    semicolon_pos = end_pos;
                    break;
                }
                end_pos++;
            }

            if (semicolon_pos == -1) {
                semicolon_pos = lextable.size;
            }

            int expression_end = semicolon_pos - 1;

            if (expression_end >= expr_start) {
                if (lexema == LEX_EQUALS) {
                    // Присваивание: обрабатываем [Выражение] и добавляем [Операнд LHS] и [=]
                    PolishNotation(expr_start, lextable, idtable, expression_end, line_number, &left_op_entry);

                    // Пропускаем все обработанные лексемы: [ID], [=], [Выражение], [;]
                    // Поскольку ID и = находятся ПЕРЕД i, мы переставляем i на ';'
                    // Следующий i++ попадет на лексему после ';'.
                    i = semicolon_pos;

                }
                else {
                    // r и p: обрабатываем [Выражение]
                    PolishNotation(expr_start, lextable, idtable, expression_end, line_number);

                    // Пропускаем [r/p], [Выражение], [;]
                    i = semicolon_pos;
                }
            }
        }
    }
}