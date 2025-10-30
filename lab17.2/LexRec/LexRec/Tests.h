#pragma once
#include "stdafx.h"
#include <locale>
#include <cwchar>
#include <tchar.h>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Out.h"
#include "LT.h"
#include "IT.h"

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


// Вспомогательные функции для тестирования IT
namespace TestIT {
	IT::Entry CreateEntry(const char*, int, IT::IDDATATYPE, IT::IDTYPE );
	void PrintTable(IT::IdTable&);
	void TestIT();
}


