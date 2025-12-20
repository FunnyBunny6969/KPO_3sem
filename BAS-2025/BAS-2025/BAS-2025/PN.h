#pragma once

#include "LT.h" 
#include "IT.h"

#define FILLER_CHAR '#'

namespace PN {
    int FindClosingParenthesisIndex(const LT::LexTable& lexTable, int start_pos);
    void FindExpressions(LT::LexTable& , IT::IdTable& );
    bool PolishNotation(int , LT::LexTable& , IT::IdTable& );
}
