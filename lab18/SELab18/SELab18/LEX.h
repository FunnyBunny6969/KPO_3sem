#pragma once
#include "LT.h"
#include "IT.h"
#include "Error.h"
#include "Automata.h"
#include "In.h"
#include <vector>
#include <string>

namespace LEX {

    struct LEX {
        LT::LexTable lexTable;
        IT::IdTable idTable;

        LEX(size_t ltSize = 1000, size_t itSize = 100) {
            lexTable = LT::Create(ltSize);
            idTable = IT::Create(itSize);
        };
    };

    void SplitIntoWords(
        const char* code, 
        std::vector<std::vector<std::string>>& lines
    );

    void Analyze(
        const char* sourceCode,
        LT::LexTable& lexTable,
        IT::IdTable& idTable
    );
}