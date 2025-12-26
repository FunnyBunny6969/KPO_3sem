#pragma once
#include "LEX.h"
#include "IT.h"
#include "LT.h"
#include "Error.h"
#include "Out.h"
#include <fstream>


namespace JS_CodeGeneration {
    void Generate(LT::LexTable lextable, IT::IdTable idtable, Out::OUT out);
}

namespace ASM_CodeGeneration {
    void Generate(LT::LexTable lextable, IT::IdTable idtable, Out::OUT out);
}
