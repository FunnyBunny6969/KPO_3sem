#pragma once
#include "LT.h"
#include "IT.h"
#include "Error.h"
#include "Automata.h"
#include "In.h"
#include <vector>
#include <string>

namespace Lexer {
    void Analyze(
        const char* sourceCode,
        LT::LexTable& lexTable,
        IT::IdTable& idTable
    );

    void SplitIntoWords(
        const char* code, 
        std::vector<std::vector<std::string>>& lines
    );
}