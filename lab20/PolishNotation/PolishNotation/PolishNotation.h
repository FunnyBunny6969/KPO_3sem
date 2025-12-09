#pragma once

#include "LT.h"
#include "IT.h"
#include <stack>
#include <vector>

namespace PN {
    bool Start(LT::LexTable& lextable, IT::IdTable& idtable);
    bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
    int GetPriority(LT::Entry entry);
}