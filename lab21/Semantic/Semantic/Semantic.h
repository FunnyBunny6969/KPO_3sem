#pragma once

#include "LT.h"
#include "IT.h"
#include "Error.h"
#include "PN.h"

namespace SemanticAnalyzer
{
    int RunSemanter(LT::LexTable&, IT::IdTable&);
}