#pragma once
#include "stdafx.h"
#include <locale>
#include <cwchar>
#include <tchar.h>
#include <cassert>
#include <vector>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Out.h"
#include "LT.h"
#include "IT.h"
#include "Automata.h"
#include "LEX.h"
#include "MFST.h"
#include "GRB.h"

void geterrorTEST();
void getparmTEST(int, _TCHAR*[]);
void getinTEST(int, _TCHAR*[]);
void getlogTEST(int, _TCHAR*[]);
void outTEST(int, _TCHAR*[]);


namespace TestLT {
    LT::Entry CreateLTEntry(char, int, int);
	void PrintLexTable(LT::LexTable&);
	IT::Entry CreateITEntry(const char*, int, IT::IDDATATYPE, IT::IDTYPE);
	void TestLT();
}

namespace TestIT {
	IT::Entry CreateEntry(const char*, int, IT::IDDATATYPE, IT::IDTYPE );
	void PrintTable(IT::IdTable&);
	void TestIT();
}


namespace TestAutomata {
    void TestSingleAutomata(
        const FST::FST& automata, 
        const char* testString, 
        bool expected, 
        const char* automataName);
    void TestKeywordAutomata();
    void TestIdentifierAutomata();
    void TestLiteralAutomata();
    void TestOperatorAutomata();
    void TestGetLexemeCode();
    void RunAllTests();
}

namespace TestLexer {
    void TestSplitter(In::IN);
}


void PrintLexTableWithSubstit(LT::LexTable& lextab, IT::IdTable& idtab);
