#pragma once
#include "LEX.h"
#include "Error.h"
#include "LT.h"
#include <stack>
#include <queue>
#include <iostream>

#define LEX_COUNT_PARM0 '0'
#define LEX_COUNT_PARM1 '1'
#define LEX_COUNT_PARM2 '2'
#define LEX_COUNT_PARM3 '3'

namespace PN
{
	bool StartPoliz(LT::LexTable& lextab, IT::IdTable& idtable);
	bool Poliz(int i, LT::LexTable& lextab, IT::IdTable& idtable);
}
