#include "Semantic.h"
#include <cstring> 
#include <iostream>
using namespace std;


namespace SemanticAnalyzer
{
	void checkFuncParms(LT::LexTable& lextable, IT::IdTable& idtable, int start) {
		IT::Entry funcInfo = idtable.table[lextable.table[start].idxTI];
		int num = 0;
		char lexema;

		for (int i = ++start; i < lextable.size; i++) {
			lexema = lextable.table[i].lexema[0];

			if (lexema == LEX_ID || lexema == LEX_LITERAL) {
				if (num > funcInfo.func_meta.n_params) 
					throw ERROR_THROW_LINE(717, lextable.table[i].sn);
				if (idtable.table[lextable.table[i].idxTI].iddatatype !=
					funcInfo.func_meta.params_types[num])
					throw ERROR_THROW_LINE(716, lextable.table[i].sn);
				num++;
			}

			if (lexema == LEX_RIGHTHESIS) {
				if (num < funcInfo.func_meta.n_params) throw ERROR_THROW_LINE(718, lextable.table[i].sn);
				return;
			}
		}
	}

	bool checkExpression(
		LT::LexTable& lextable,
		IT::IdTable& idtable,
		int start,
		IT::IDDATATYPE type = IT::UNDEF,
		bool switchExp = false) 
	{
		char lexema;
		IT::Entry info;
		bool inFunc = false;
		int hesisBalance = 0;

		int t = start;
		if (type == IT::UNDEF) {
			for (int i = ++t; i < lextable.size; i++) {
				lexema = lextable.table[i].lexema[0];
				if(lextable.table[i].idxTI >= 0)
					info = idtable.table[lextable.table[i].idxTI];
				if (lexema == LEX_ID || lexema == LEX_LITERAL) {
					type = info.iddatatype;
					break;
				}
			}
		}
		for (int i = ++start; i < lextable.size; i++) {
			lexema = lextable.table[i].lexema[0];

			if(lextable.table[i].idxTI >= 0)
				info = idtable.table[lextable.table[i].idxTI];

			if ((lexema == LEX_ID || lexema == LEX_LITERAL) && !inFunc) {
				if (info.iddatatype != type)
					return false;

				if (info.idtype == IT::F)
					inFunc = true;
			}

			if (lexema == LEX_LEFTHESIS) {
				hesisBalance += 1;
			}
			if (lexema == LEX_RIGHTHESIS) {
				inFunc = false;
				hesisBalance -= 1;

				if (switchExp && hesisBalance == 0) {
					if (lextable.table[i+1].lexema[0] == LEX_LEFTBRACE)
						return true;
				}
			}


			if ((type == IT::CHAR || type == IT::STR) &&
				(lexema == LEX_PLUS ||
				lexema == LEX_MINUS ||
				lexema == LEX_STAR ||
				lexema == LEX_DIRSLASH))
				throw ERROR_THROW_LINE(712, lextable.table[i].sn);

			if ((type == IT::CHAR || type == IT::STR) &&
				(lexema == LEX_BIT_AND ||
				lexema == LEX_BIT_NOT ||
				lexema == LEX_BIT_OR))
				throw ERROR_THROW_LINE(713, lextable.table[i].sn);

			if(lexema == LEX_LITERAL && info.iddatatype == IT::UINT)
				if (info.value.vint == 0)
					if(lextable.table[i-1].lexema[0] == LEX_DIRSLASH)
						throw ERROR_THROW_LINE(714, lextable.table[i].sn);

			if (lexema == LEX_SEMICOLON) {
				return true;
			}

		}
		return true;
	}


	int RunSemanter(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		char lexema;
		LT::Entry entry;
		int mainNum = 0;


		for (int i = 0; i < lextable.size; i++)
		{
			entry = lextable.table[i];
			lexema = entry.lexema[0];


			switch (lexema)
			{


			case LEX_MAIN: 
				mainNum += 1;
				if(mainNum > 1)
					throw ERROR_THROW_LINE(702, entry.sn);
				break;



			case LEX_ID:
				if (idtable.table[entry.idxTI].idtype == IT::F) {
					if (idtable.table[entry.idxTI].func_meta.n_params > IT_MAX_PARAMS)
						throw ERROR_THROW_LINE(706, entry.sn);
					if (lextable.table[i - 1].lexema[0] != LEX_FUNCTION)
						checkFuncParms(lextable, idtable, i);

				}
				break;




			case LEX_EQUALS:
				if(!checkExpression(lextable, idtable, i, idtable.table[lextable.table[i - 1].idxTI].iddatatype))
					throw ERROR_THROW_LINE(710, entry.sn);
				break;


			case LEX_PRINT:
			case LEX_RETURN:
				if(!checkExpression(lextable, idtable, i))
					throw ERROR_THROW_LINE(711, entry.sn);

				break;


			case LEX_SWITCH:
				if(!checkExpression(lextable, idtable, i, IT::UNDEF, true))
					throw ERROR_THROW_LINE(711, entry.sn);
				break;


			}

		}



		return true;
	}
}
