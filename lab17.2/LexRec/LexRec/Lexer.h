#pragma once
#include "LT.h"
#include "IT.h"
#include "Error.h"
#include <vector>
#include <string>

namespace Lexer {
    void Analyze(
        const char* sourceCode,
        LT::LexTable& lexTable,
        IT::IdTable& idTable,
        std::vector<Error::ERROR>& errors
    );

    void SplitIntoWords(const char* code, std::vector<std::string>& words);
    char GetTokenCode(const std::string& text);
    bool IsDelimiter(char c);
}