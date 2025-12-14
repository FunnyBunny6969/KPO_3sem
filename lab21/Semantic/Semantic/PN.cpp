#include "PN.h"
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <sstream>
using namespace std;

namespace PN {
    int GetPriority(LT::Entry entry, bool isFunc = false)
    {
        // Функции имеют самый высокий приоритет, чтобы оставаться в стеке
        if (isFunc) return 7;

        char lex = entry.lexema[0];
        switch (lex)
        {
            // Унарная инверсия (~) — Приоритет 6 (Очень высокий)
        case LEX_BIT_NOT:
            return 6;

            // Высший приоритет для мат. операторов (5)
        case LEX_STAR:
        case LEX_DIRSLASH:
            return 5;

            // Средний приоритет для мат. операторов (4)
        case LEX_PLUS:
        case LEX_MINUS:
            return 4;

        case LEX_BIT_AND: 
            return 3;

        case LEX_EQUALS:
        case LEX_PRINT:
        case LEX_RETURN:
        case LEX_BIT_OR:  
        //case LEX_IF:
        //case LEX_ELSE:
            return 2; // Самый низкий приоритет (должны выходить последними)

        case LEX_COMMA:
            return 1; // Разделитель аргументов


			// Скобки и неизвестные лексемы
        case LEX_LEFTHESIS:
        case LEX_RIGHTHESIS:
            return 0;

        default:
            return 0; // Неизвестный оператор или операнд
        }
    }


    int FindClosingParenthesisIndex(const LT::LexTable& lexTable, int start_pos)
    {
        int balance = 1;
        int currentIndex = start_pos + 1; 

        while (currentIndex < lexTable.size)
        {
            const LT::Entry& currentLexeme = lexTable.table[currentIndex];
            char lexChar = currentLexeme.lexema[0];

            if (lexChar == '(') {
                balance++;
            }
            else if (lexChar == ')') {
                balance--;
            }

            if (balance == 0) {
                return currentIndex; 
            }
            currentIndex++;
        }
        return 0;
    }


    bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
    {
        stack<pair<LT::Entry, int>> stack;
        vector<pair<LT::Entry, int>> out_buffer;

        bool switchExp = false;
		if (lextable_pos - 2 >= 0)
			if (lextable.table[lextable_pos - 2].lexema[0] == LEX_SWITCH)
				switchExp = true;


        // Находим конец выражения
        int end_pos;
        if (!switchExp) {
			end_pos = lextable_pos;
			while (end_pos < lextable.size && lextable.table[end_pos].lexema[0] != LEX_SEMICOLON)
			{
				end_pos++;
			}
        }
        else {
            end_pos = FindClosingParenthesisIndex(lextable, lextable_pos-1);
            if (end_pos == 0) return false;
        }
        if (end_pos == lextable.size) return false;


		cout << "ВХОДНАЯ СТРОКА: ";
		for (int i = lextable_pos; i < end_pos; i++)
			cout << lextable.table[i].lexema[0];
		cout << endl;


        //Алгоритм Shunting-yard
        for (int i = lextable_pos; i < end_pos; ++i)
        {
            LT::Entry curr = lextable.table[i];
            char lex = curr.lexema[0];

            switch (lex)
            {
                // A. Операнды: ID (переменная) и LITERAL
            case LEX_ID:
            case LEX_LITERAL:
            {
                if (lex == LEX_ID)
                {
                    IT::Entry itEntry = IT::GetEntry(idtable, curr.idxTI);
                    // Если это функция, помещаем ее в стек операторов
                    if (itEntry.idtype == IT::F)
                    {
                        int pCount = idtable.table[curr.idxTI].func_meta.n_params;
                        stack.push({ curr, pCount });
                        continue;
                    }
                }
                out_buffer.push_back({ curr, -1 }); // Операнд в буфер
                break;
            }

            // B. Скобки и разделители
            case LEX_LEFTHESIS:
            {
                stack.push({ curr, -1 });
                break;
            }

            case LEX_RIGHTHESIS:
            {
                // Выталкиваем операторы до '('
                while (!stack.empty() && stack.top().first.lexema[0] != LEX_LEFTHESIS)
                {
                    out_buffer.push_back(stack.top());
                    stack.pop();
                }

                if (!stack.empty()) stack.pop(); // Удаляем '('

                // Если на вершине стека была функция, выталкиваем ее
                if (!stack.empty())
                {
                    pair<LT::Entry, int> top = stack.top();
                    if (top.first.lexema[0] == LEX_ID)
                    {
                        IT::Entry itEntry = IT::GetEntry(idtable, top.first.idxTI);
                        if (itEntry.idtype == IT::F)
                        {
                            out_buffer.push_back(top); // Функция идет после аргументов
                            stack.pop();
                        }
                    }
                }
                break;
            }

            case LEX_COMMA:
            {
                // Выталкиваем операторы до '(' (разделитель аргументов)
                while (!stack.empty() && stack.top().first.lexema[0] != LEX_LEFTHESIS)
                {
                    out_buffer.push_back(stack.top());
                    stack.pop();
                }
                break;
            }

            // C. Операторы (включая мат. операторы)
            case LEX_PLUS:
            case LEX_MINUS:
            case LEX_STAR:
            case LEX_DIRSLASH:
            case LEX_EQUALS:
            case LEX_PRINT:
            case LEX_RETURN:
            case LEX_BIT_AND:
            case LEX_BIT_OR:
            case LEX_BIT_NOT:
            //case LEX_IF:
            //case LEX_ELSE:
            {
                while (!stack.empty())
                {
                    pair<LT::Entry, int> top = stack.top();
                    char topLex = top.first.lexema[0];

                    // Проверка, является ли вершина стека функцией
                    bool topIsFunc = (topLex == LEX_ID && IT::GetEntry(idtable, top.first.idxTI).idtype == IT::F);

                    // Условие для выталкивания: Приоритет вершины >= Приоритета текущей лексемы
                    if (GetPriority(top.first, topIsFunc) >= GetPriority(curr))
                    {
                        // Операторы, которые останавливают выталкивание: '('
                        if (topLex == LEX_LEFTHESIS) break;

                        out_buffer.push_back(top);
                        stack.pop();
                    }
                    else break;
                }
                stack.push({ curr, -1 });
                break;
            }
            default: break;
            }
        }

        // Очистка стека
        while (!stack.empty())
        {
            // Если в стеке осталась скобка, это ошибка
            if (stack.top().first.lexema[0] == LEX_LEFTHESIS) return false;
            out_buffer.push_back(stack.top());
            stack.pop();
        }

        // Запись результатов в ТЛ и заполнение '#'
        int out_idx = 0;
        for (int i = lextable_pos; i < end_pos; ++i)
        {
            if (out_idx < out_buffer.size())
            {
                lextable.table[i] = out_buffer[out_idx++].first;
            }
            else
            {
                lextable.table[i].lexema[0] = FILLER_CHAR;
                lextable.table[i].idxTI = LT_TI_NULLIDX;
                lextable.table[i].sn = lextable.table[end_pos].sn;
            }
        }

        // блок cout для печати ОПЗ
        cout << "Польская запись (строка " << lextable.table[lextable_pos].sn << "): ";
        for (const auto& item : out_buffer)
        {
            LT::Entry entry = item.first;
            int pCount = item.second;

            if (entry.lexema[0] == LEX_ID || entry.lexema[0] == LEX_LITERAL)
            {
                IT::Entry itEntry = IT::GetEntry(idtable, entry.idxTI);
                if (itEntry.idtype == IT::L)
                {
                    if (itEntry.iddatatype == IT::STR) cout << "'" << itEntry.value.vstr->str << "' ";
                    else cout << itEntry.value.vint << " ";
                }
                else
                {
                    cout << itEntry.id;
                    if (itEntry.idtype == IT::F && pCount != -1) cout << "@" << pCount;
                    cout << " ";
                }
            }
            else
            {
                cout << entry.lexema[0] << " ";
            }
        }
        cout << endl;

        return true;
    }


    void FindExpressions(LT::LexTable& lextable, IT::IdTable& idtable) {
        std::cout << "\n=== ПРЕОБРАЗОВАНИЕ В ПОЛИЗ ===\n";
        bool switchExp = false;

        for (int i = 0; i < lextable.size; ++i) {
            char lexema = lextable.table[i].lexema[0];
            if (lexema == FILLER_CHAR) continue;

            if (i - 2 >= 0)
                if (lextable.table[i - 2].lexema[0] == LEX_SWITCH)
                    switchExp = true;

            if ((lexema == LEX_ID && 
                i + 1 < lextable.size && 
                lextable.table[i + 1].lexema[0] == LEX_EQUALS) ||

                lexema == LEX_RETURN || 
                lexema == LEX_PRINT  ||
                switchExp) 
            {
                int expr_start = i;



                PolishNotation(expr_start, lextable, idtable);
            }

            switchExp = false;
        }
    }
}