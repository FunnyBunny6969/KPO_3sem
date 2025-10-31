#pragma once
#include "LT.h"
#include "IT.h" 
#include "In.h"
#include "Error.h"
#include "Log.h"

namespace Lexer {

    // Результат лексического анализа - ТОЛЬКО таблицы
    struct Result {
        LT::LexTable lexTable;
        IT::IdTable idTable;

        Result() {
            lexTable = LT::Create(LT_MAXSIZE);
            idTable = IT::Create(TI_MAXSIZE);
        }
    };

    // Основные функции лексического анализа
    Result Analyze(In::IN input, Log::LOG log);
}