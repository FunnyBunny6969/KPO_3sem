#include "PolishNotation.h"
#include <stack>
#include <queue>
#include <iostream>

namespace PN
{
    int GetPriority(char lexema)
    {
        switch (lexema)
        {
        case LEX_LEFTHESIS:
        case LEX_RIGHTHESIS:
            return 0;
        case LEX_COMMA:
            return 1;
        case LEX_EQUALS:
        case LEX_RETURN:
        case LEX_PRINT:
            return 2;
        case LEX_PLUS:
        case LEX_MINUS:
            return 4;
        case LEX_STAR:
        case LEX_DIRSLASH:
            return 5;
        default:
            return -1;
        }
    }

    bool IsOperator(char lexema)
    {
        return lexema == LEX_PLUS || lexema == LEX_MINUS ||
            lexema == LEX_STAR || lexema == LEX_DIRSLASH ||
            lexema == LEX_EQUALS;
    }

    // Обработка объявления функции: заменяет (ti,ti) на $N
    void ProcessFunctionDeclaration(LT::LexTable& lextab, int startPos)
    {
        int i = startPos;

        // Ищем открывающую скобку
        while (i < lextab.size && lextab.table[i].lexema[0] != LEX_LEFTHESIS)
            i++;

        if (i >= lextab.size || lextab.table[i].lexema[0] != LEX_LEFTHESIS)
            return;

        int openParen = i;
        int closeParen = -1;
        int paramCount = 0;

        // Ищем закрывающую скобку
        for (int j = openParen; j < lextab.size; j++)
        {
            if (lextab.table[j].lexema[0] == LEX_RIGHTHESIS)
            {
                closeParen = j;
                break;
            }
            if (lextab.table[j].lexema[0] == LEX_COMMA)
            {
                paramCount++;
            }
        }

        if (closeParen != -1)
        {
            // Учитываем последний параметр
            if (lextab.table[openParen + 1].lexema[0] != LEX_RIGHTHESIS)
                paramCount++;

            // Заменяем '(' на '$'
            lextab.table[openParen].lexema[0] = '$';
            lextab.table[openParen].idxTI = paramCount;

            // Очищаем всё между скобками
            for (int k = openParen + 1; k <= closeParen; k++)
            {
                lextab.table[k].lexema[0] = '~';
                lextab.table[k].idxTI = LT_TI_NULLIDX;
            }
        }
    }

    bool Poliz(int startPos, LT::LexTable& lextab, IT::IdTable& idtable)
    {
        std::stack<LT::Entry> stack;
        std::queue<LT::Entry> output;

        // Находим конец выражения
        int endPos = startPos;
        while (endPos < lextab.size && lextab.table[endPos].lexema[0] != LEX_SEMICOLON)
            endPos++;

        if (endPos >= lextab.size)
            return false;

        bool inFunctionCall = false;
        LT::Entry functionEntry;
        int paramCount = 0;
        bool hasFirstParam = false;

        for (int i = startPos; i < endPos; i++)
        {
            LT::Entry entry = lextab.table[i];
            char lex = entry.lexema[0];

            // 1. Идентификаторы и литералы
            if (lex == LEX_ID || lex == LEX_LITERAL)
            {
                // Проверяем, не функция ли это (смотрим следующий токен)
                bool isFunction = false;
                if (lex == LEX_ID && i + 1 < endPos &&
                    lextab.table[i + 1].lexema[0] == LEX_LEFTHESIS)
                {
                    if (entry.idxTI != LT_TI_NULLIDX && entry.idxTI < idtable.size)
                    {
                        IT::Entry idEntry = idtable.table[entry.idxTI];
                        if (idEntry.idtype == IT::F)
                        {
                            isFunction = true;
                            inFunctionCall = true;
                            functionEntry = entry;
                            paramCount = 0;
                            hasFirstParam = false;
                        }
                    }
                }

                if (!isFunction)
                {
                    output.push(entry);
                    if (inFunctionCall && !hasFirstParam)
                    {
                        hasFirstParam = true;
                        paramCount = 1;
                    }
                }
            }
            // 2. Операторы
            else if (IsOperator(lex))
            {
                while (!stack.empty() && stack.top().lexema[0] != LEX_LEFTHESIS &&
                    GetPriority(lex) <= GetPriority(stack.top().lexema[0]))
                {
                    output.push(stack.top());
                    stack.pop();
                }
                stack.push(entry);
            }
            // 3. Открывающая скобка
            else if (lex == LEX_LEFTHESIS)
            {
                stack.push(entry);
            }
            // 4. Запятая
            else if (lex == LEX_COMMA)
            {
                if (inFunctionCall)
                    paramCount++;

                // Выталкиваем операторы до '('
                while (!stack.empty() && stack.top().lexema[0] != LEX_LEFTHESIS)
                {
                    output.push(stack.top());
                    stack.pop();
                }
            }
            // 5. Закрывающая скобка
            else if (lex == LEX_RIGHTHESIS)
            {
                // Выталкиваем операторы до '('
                while (!stack.empty() && stack.top().lexema[0] != LEX_LEFTHESIS)
                {
                    output.push(stack.top());
                    stack.pop();
                }

                // Удаляем '('
                if (!stack.empty() && stack.top().lexema[0] == LEX_LEFTHESIS)
                    stack.pop();

                // Если это был вызов функции
                if (inFunctionCall)
                {
                    // Добавляем функцию ПОСЛЕ аргументов
                    output.push(functionEntry);

                    // Добавляем маркер вызова
                    LT::Entry atMarker;
                    atMarker.lexema[0] = '@';
                    atMarker.idxTI = functionEntry.idxTI;
                    atMarker.sn = functionEntry.sn;
                    output.push(atMarker);

                    // Добавляем количество параметров
                    LT::Entry paramEntry;
                    if (paramCount < 10)
                        paramEntry.lexema[0] = '0' + paramCount;
                    else
                        paramEntry.lexema[0] = 'A' + (paramCount - 10);
                    paramEntry.idxTI = paramCount;
                    paramEntry.sn = functionEntry.sn;
                    output.push(paramEntry);

                    inFunctionCall = false;
                }
            }
            // 6. Остальное (d, t, r, p без скобок)
            else
            {
                output.push(entry);
            }
        }

        // Выталкиваем оставшиеся операторы
        while (!stack.empty())
        {
            if (stack.top().lexema[0] == LEX_LEFTHESIS ||
                stack.top().lexema[0] == LEX_RIGHTHESIS)
                return false;

            output.push(stack.top());
            stack.pop();
        }

        // Записываем результат
        int outPos = startPos;
        while (!output.empty())
        {
            if (outPos < lextab.size)
            {
                lextab.table[outPos] = output.front();
                outPos++;
            }
            output.pop();
        }

        // Добавляем точку с запятой
        if (outPos < lextab.size)
        {
            lextab.table[outPos] = lextab.table[endPos];
            outPos++;
        }

        // Очищаем остаток
        for (int i = outPos; i <= endPos; i++)
        {
            if (i < lextab.size)
            {
                lextab.table[i].lexema[0] = '~';
                lextab.table[i].idxTI = LT_TI_NULLIDX;
            }
        }

        return true;
    }

    bool StartPoliz(LT::LexTable& lextab, IT::IdTable& idtable)
    {
        // 1. Обработка объявлений функций
        for (int i = 0; i < lextab.size; i++)
        {
            // Объявление функции: t f i ( ... )
            if (i + 3 < lextab.size &&
                lextab.table[i].lexema[0] == LEX_INTEGER &&
                lextab.table[i + 1].lexema[0] == LEX_FUNCTION &&
                lextab.table[i + 2].lexema[0] == LEX_ID)
            {
                ProcessFunctionDeclaration(lextab, i);
            }
            // Объявление переменной типа функция: d t f i ( ... )
            else if (i + 4 < lextab.size &&
                lextab.table[i].lexema[0] == LEX_DECLARE &&
                lextab.table[i + 1].lexema[0] == LEX_INTEGER &&
                lextab.table[i + 2].lexema[0] == LEX_FUNCTION &&
                lextab.table[i + 3].lexema[0] == LEX_ID)
            {
                ProcessFunctionDeclaration(lextab, i);
            }
        }

        // 2. Преобразование выражений в польскую запись
        for (int i = 0; i < lextab.size; i++)
        {
            char lex = lextab.table[i].lexema[0];

            // Выражения после =, r, p
            if (lex == LEX_EQUALS || lex == LEX_RETURN ||
                (lex == LEX_PRINT && i + 1 < lextab.size &&
                    lextab.table[i + 1].lexema[0] == LEX_LEFTHESIS))
            {
                if (!Poliz(i + 1, lextab, idtable))
                    return false;

                // Пропускаем обработанное
                while (i < lextab.size && lextab.table[i].lexema[0] != LEX_SEMICOLON)
                    i++;
            }
            // Вызов функции как отдельное выражение
            else if (lex == LEX_ID && i + 1 < lextab.size &&
                lextab.table[i + 1].lexema[0] == LEX_LEFTHESIS)
            {
                if (lextab.table[i].idxTI != LT_TI_NULLIDX &&
                    lextab.table[i].idxTI < idtable.size)
                {
                    IT::Entry idEntry = idtable.table[lextab.table[i].idxTI];
                    if (idEntry.idtype == IT::F)
                    {
                        if (!Poliz(i, lextab, idtable))
                            return false;

                        while (i < lextab.size && lextab.table[i].lexema[0] != LEX_SEMICOLON)
                            i++;
                    }
                }
            }
        }

        return true;
    }
}