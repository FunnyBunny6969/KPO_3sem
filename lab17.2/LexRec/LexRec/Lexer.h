#pragma once
#include "LT.h"
#include "IT.h"
#include "In.h"
#include "FST.h"
#include "Error.h"
#include "Log.h"

namespace Lexer {
    struct RESULT {
        LT::LexTable lextable;
        IT::IdTable idtable;
    };

    // Главная функция лексического анализа
    RESULT Analyze(In::IN, Log::LOG);

    namespace Internal {
        // Разбивает текст на лексемы и обрабатывает каждую
        void ProcessText(const char*, RESULT&, Log::LOG);

        // Выделяет одну лексему из текста
        void ExtractToken(const char*&, char*);

        // Распознает ОДНУ лексему всеми автоматами
        bool RecognizeToken(const char*, int, int, RESULT&, Log::LOG);

        // Добавляет распознанную лексему в таблицы
        void AddToTables(RESULT&, char, const char*, int, int, Log::LOG);

        // Добавляет идентификатор в IT
        void AddIdentifier(RESULT&, const char*, int, int, Log::LOG);
    }
}
