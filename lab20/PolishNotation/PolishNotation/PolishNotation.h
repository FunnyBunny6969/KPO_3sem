#pragma once

#include "LT.h"
#include "IT.h"

namespace PN {

    // ГЛАВНАЯ функция преобразования
    bool PolishNotation(
        int lextable_pos,      // позиция ПЕРВОГО символа выражения
        LT::LexTable& lextable,
        IT::IdTable& idtable
    );

    // Вспомогательная: преобразовать все выражения в файле
    bool ConvertAllExpressions(LT::LexTable& lextable, IT::IdTable& idtable);
    
    // Нормализовать таблицу лексем после преобразований
    // Удаляет заполнители, сдвигает лексемы, обновляет ссылки
    void NormalizeLexTable(LT::LexTable& lextable, IT::IdTable& idtable);
}