#include "PolishNotation.h"
#include <iostream>
#include <stack>
using namespace std;

namespace PN
{
    // Приоритеты операций 
    // 0: ( )
    // 1: , (запятая)
    // 2: = yield display
    // 3: & > < @ # (сравнения)
    // 4: + -
    // 5: * / %
    int GetPriority(LT::Entry entry)
    {
        switch (entry.lexema[0])
        {
        case LEX_LEFTHESIS: // (
        case LEX_RIGHTHESIS: // )
            return 0;

        case LEX_COMMA:// ,
            return 1;

        case LEX_EQUALS: // =
        case LEX_RETURN:   // yield (return)
        case LEX_PRINT: // display (print)
            return 2;

        case LEX_MORE:   // >
        case LEX_LESS:   // <
        case LEX_LE:    // @ (<=)
        case LEX_GE:    // # (>=)
        case LEX_EQUAL:     // & (==)
        case LEX_NOTEQUAL: // ! (!=)
            return 3;

        case LEX_PLUS:  // +
        case LEX_MINUS: // -
            return 4;

        case LEX_STAR:    // *
        case LEX_DIRSLASH: // /
        case LEX_MOD:    // %
            return 5;

        default:
            return -1;
        }
    }

    bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable)
    {
        std::stack<LT::Entry> stack;
        std::vector<LT::Entry> out_string;
        std::stack<int> args_count;

        int expr_end = lextable_pos;
        while (expr_end < lextable.size && lextable.table[expr_end].lexema[0] != LEX_SEMICOLON) {
            expr_end++;
        }
        if (expr_end >= lextable.size) return false;

        for (int i = lextable_pos; i < expr_end; i++)
        {
            LT::Entry entry = lextable.table[i];

            if (entry.lexema == 0) continue;

            switch (entry.lexema[0])
            {
            case LEX_ID:
            case LEX_LITERAL:
            {
                bool isFunction = false;
                if (entry.lexema[0] == LEX_ID && entry.idxTI != LT_TI_NULLIDX) {
                    IT::Entry idEntry = IT::GetEntry(idtable, entry.idxTI);
                    if (idEntry.idtype == IT::F) {
                        isFunction = true;
                    }
                }

                if (isFunction) {
                    stack.push(entry);
                }
                else {
                    out_string.push_back(entry);
                }
                break;
            }

            case LEX_LEFTHESIS:
            {
                if (!stack.empty()) {
                    LT::Entry top = stack.top();
                    if (top.lexema[0] == LEX_ID && top.idxTI != LT_TI_NULLIDX) {
                        IT::Entry idEntry = IT::GetEntry(idtable, top.idxTI);
                        if (idEntry.idtype == IT::F) {
                            if (i + 1 < expr_end && lextable.table[i + 1].lexema[0] == LEX_RIGHTHESIS) {
                                args_count.push(0);
                            }
                            else {
                                args_count.push(1);
                            }
                        }
                    }
                }
                stack.push(entry);
                break;
            }

            case LEX_COMMA:
            {
                while (!stack.empty() && stack.top().lexema[0] != LEX_LEFTHESIS)
                {
                    out_string.push_back(stack.top());
                    stack.pop();
                }
                if (!args_count.empty()) {
                    args_count.top()++;
                }
                break;
            }

            case LEX_RIGHTHESIS:
            {
                while (!stack.empty() && stack.top().lexema[0] != LEX_LEFTHESIS)
                {
                    out_string.push_back(stack.top());
                    stack.pop();
                }

                if (stack.empty()) return false;
                stack.pop();

                if (!stack.empty())
                {
                    LT::Entry top = stack.top();
                    if (top.lexema[0] == LEX_ID && top.idxTI != LT_TI_NULLIDX)
                    {
                        IT::Entry idEntry = IT::GetEntry(idtable, top.idxTI);
                        if (idEntry.idtype == IT::F)
                        {
                            stack.pop();
                            int params = 0;
                            if (!args_count.empty()) {
                                params = args_count.top();
                                args_count.pop();
                            }
                            top.lexema[0] = '$';
                            top.idxTI = params;
                            out_string.push_back(top);
                        }
                    }
                }
                break;
            }


            case LEX_PLUS: case LEX_MINUS: case LEX_STAR: case LEX_DIRSLASH:
            case LEX_MOD: case LEX_EQUALS: case LEX_MORE: case LEX_LESS:
            case LEX_LE: case LEX_GE: case LEX_EQUAL: case LEX_NOTEQUAL:
            {
                while (!stack.empty() && stack.top().lexema[0] != LEX_LEFTHESIS &&
                    (GetPriority(stack.top()) >= GetPriority(entry)))
                {
                    out_string.push_back(stack.top());
                    stack.pop();
                }
                stack.push(entry);
                break;
            }
            }
        }

        while (!stack.empty())
        {
            if (stack.top().lexema[0] == LEX_LEFTHESIS || stack.top().lexema[0] == LEX_RIGHTHESIS)
                return false;
            out_string.push_back(stack.top());
            stack.pop();
        }

        int outIndex = 0;
        for (const auto& rpnEntry : out_string)
        {
            lextable.table[lextable_pos + outIndex] = rpnEntry;
            outIndex++;
        }

        for (int i = lextable_pos + outIndex; i < expr_end; i++)
        {
            lextable.table[i].lexema[0] = 0;
            lextable.table[i].idxTI = LT_TI_NULLIDX;
        }

        return true;
    }

    bool Start(LT::LexTable& lextable, IT::IdTable& idtable)
    {
        for (int i = 0; i < lextable.size; i++)
        {
            if (lextable.table[i].lexema[0] == LEX_EQUALS ||
                lextable.table[i].lexema[0] == LEX_RETURN ||
                lextable.table[i].lexema[0] == LEX_PRINT)
            {
                if (!PolishNotation(i + 1, lextable, idtable)) {
                    return false;
                }
            }
            else if (lextable.table[i].lexema[0] == LEX_FUNCTION)
            {
                if (i + 2 < lextable.size && lextable.table[i + 2].lexema[0] == LEX_LEFTHESIS)
                {
                    int openParenPos = i + 2;
                    int closeParenPos = -1;
                    int paramCount = 0;

                    if (lextable.table[openParenPos + 1].lexema[0] == LEX_RIGHTHESIS)
                    {
                        paramCount = 0;
                        closeParenPos = openParenPos + 1;
                    }
                    else
                    {
                        paramCount = 1;
                        for (int k = openParenPos + 1; k < lextable.size; k++)
                        {
                            if (lextable.table[k].lexema[0] == LEX_RIGHTHESIS)
                            {
                                closeParenPos = k;
                                break;
                            }
                            if (lextable.table[k].lexema[0] == LEX_COMMA)
                            {
                                paramCount++;
                            }
                        }
                    }

                    if (closeParenPos == -1) return false;

                    lextable.table[openParenPos].lexema[0] = '$';
                    lextable.table[openParenPos].idxTI = paramCount;

                    for (int k = openParenPos + 1; k <= closeParenPos; k++)
                    {
                        lextable.table[k].lexema[0] = 0;
                        lextable.table[k].idxTI = LT_TI_NULLIDX;
                    }
                }
            }
        }
        return true;
    }
}