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



    void ConvertFunctionCall(LT::LexTable& lextable, IT::IdTable& idtable,
        int start, int end, int argCount);
	void ConvertSimpleExpression(LT::LexTable & lextable, IT::IdTable & idtable,
		int start, int end);
    bool PolishNotationRecursive(LT::LexTable& lextable, IT::IdTable& idtable,
        int start, int end, bool);
    // ================== СКАНЕР ==================
    struct ScanResult {
        int type;        // 1=присваивание, 2=функция, 3=арифметика
        int start;       // начало выражения
        int end;         // конец выражения (не включая символ конца)
        char functionName; // для вызовов функций
    };
    ScanResult Scanner(const LT::LexTable& lextable, int start, int end);
}