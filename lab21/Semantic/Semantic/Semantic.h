#pragma once

#include "LT.h"
#include "IT.h"
#include "Error.h"
#include "PN.h"

namespace SemanticAnalyzer
{
    void RunSemanter(LT::LexTable& lextable, IT::IdTable& idtable);
}