#include "Sem.h"
#include "In.h"
#include <cstring> 
#include <iostream>
#include <vector>
#include <unordered_set>
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
				num += 1;

				if (num > funcInfo.func_meta.n_params) 
					throw ERROR_THROW_LINE(717, lextable.table[i].sn);

				if (idtable.table[lextable.table[i].idxTI].iddatatype !=
					funcInfo.func_meta.params_types[num-1])
					throw ERROR_THROW_LINE(716, lextable.table[i].sn);

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


	void checkIdTable(IT::IdTable& idtable) {
		std::unordered_set<std::string> functionNames;

		for (int i = 0; i < idtable.size; i++) {
			IT::Entry& entry = idtable.table[i];


			if (entry.idtype == IT::L &&
				entry.iddatatype == IT::STR)
				if (entry.value.vstr->str[0] == IN_CODE_ENDS)
					throw ERROR_THROW(708);


			if (entry.idtype == IT::F) {
				std::string funcName(entry.id);
				if (functionNames.find(funcName) != functionNames.end())
					throw ERROR_THROW(705);

				functionNames.insert(funcName);
			}
		}
	}


	void checkFuncBody(LT::LexTable& lextable, IT::IdTable& idtable, int start) {

		LT::Entry entry = lextable.table[start];
		IT::IDDATATYPE datatype;
		IT::Entry info;
		if (entry.lexema[0] != LEX_MAIN)
			datatype = idtable.table[entry.idxTI].iddatatype;
		else
			datatype = IT::UNDEF;
		vector<string> validIdentifiers;
		vector<IT::IDTYPE>types;

		int braceCount = 0;
		int returnCount = 0;
		char lexema;


		for (int i = start; i < lextable.size; i++) {
			entry = lextable.table[i];
			lexema = entry.lexema[0];

			if (lexema == LEX_RETURN) {
				for (int j = i+1; j < lextable.size; j++) {
					if (lextable.table[j].lexema[0] == LEX_ID)
						if (datatype !=
							idtable.table[lextable.table[j].idxTI].iddatatype)
							throw ERROR_THROW_LINE(715, entry.sn);

					if (lextable.table[j].lexema[0] == LEX_SEMICOLON)
						break;
				}

				returnCount++;
			}


			if (lexema == LEX_LEFTBRACE) braceCount++;
			if (lexema == LEX_RIGHTBRACE) {
				braceCount--;
				if (braceCount == 0) {
					if (returnCount > 0) return;
					else throw ERROR_THROW_LINE(723, entry.sn);
				}

			}


			if (lexema == LEX_ID) {
				info = idtable.table[entry.idxTI];
				if (lextable.table[i - 1].lexema[0] == LEX_STRING ||
					lextable.table[i - 1].lexema[0] == LEX_UINT ||
					lextable.table[i - 1].lexema[0] == LEX_CHAR)
				{
					bool exists = 
						std::find(
							validIdentifiers.begin(), 
							validIdentifiers.end(), 
							info.id) 
						!= validIdentifiers.end();

					if (!exists) { 
						validIdentifiers.push_back(info.id);
						types.push_back(info.idtype);
					}
					else {
						int num;
						for (size_t i = 0; i < validIdentifiers.size(); i++) {
							if (validIdentifiers[i] == info.id) {
								num = i;
								break;
							}
						}

						if (types[num] == IT::P) throw ERROR_THROW_LINE(709, entry.sn);
						throw ERROR_THROW_LINE(704, entry.sn);

					}
				}
			}
		}
	}




	void checkSwitch(LT::LexTable& lextable, IT::IdTable& idtable, int start) {

		LT::Entry entry = lextable.table[start];
		IT::IDDATATYPE datatype = IT::UNDEF;
		IT::Entry info;

		int braceCount = 0;
		int defCount = 0;
		char lexema;
		bool ExpEnd = false;


		for (int i = start; i < lextable.size; i++) {
			entry = lextable.table[i];
			lexema = entry.lexema[0];

			if ((lexema == LEX_ID || lexema == LEX_LITERAL) 
				&& !ExpEnd) 
			{
				info = idtable.table[entry.idxTI];
				if (datatype == IT::UNDEF) datatype = info.iddatatype;
			}


			if (lexema == LEX_LEFTBRACE) braceCount += 1;
			if (lexema == LEX_RIGHTBRACE) {
				braceCount -= 1;
				if (braceCount == 0) {
					return;
				}
			}

			if (braceCount == 1) {
				if (lexema == LEX_LITERAL &&
					lextable.table[i - 1].lexema[0] == LEX_CASE) 
				{
					info = idtable.table[entry.idxTI];
					if (info.iddatatype != datatype) throw ERROR_THROW_LINE(719, entry.sn);
				}

				if (lexema == LEX_RIGHTHESIS) ExpEnd = true;

				if (lexema == LEX_DEFAULT) {
					defCount += 1;
					if (defCount > 1) throw ERROR_THROW_LINE(720, entry.sn);
				}
			}
		}
	}




	int RunSemanter(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		char lexema;
		LT::Entry entry;
		int mainNum = 0;        
        std::vector<int> scope;
        scope.push_back(GLOBAL_SCOPE);
		int reserve = 0;

		
		checkIdTable(idtable);
		for (int i = 0; i < lextable.size; i++)
		{
			entry = lextable.table[i];
			lexema = entry.lexema[0];


			switch (lexema)
			{
			case LEX_MAIN: 
				scope.push_back(MAIN_SCOPE);
				checkFuncBody(lextable, idtable, i);
				mainNum += 1;
				if(mainNum > 1)
					throw ERROR_THROW_LINE(702, entry.sn);
				break;


			case LEX_RIGHTBRACE:
				if (reserve != 0) reserve--;
				else scope.pop_back();
				break;


			case LEX_ID:
				if (idtable.table[entry.idxTI].idtype == IT::F) {
					scope.push_back(entry.idxTI);

					if (idtable.table[entry.idxTI].func_meta.n_params > IT_MAX_PARAMS)
						throw ERROR_THROW_LINE(706, entry.sn);

					if (lextable.table[i - 1].lexema[0] != LEX_FUNCTION)
						checkFuncParms(lextable, idtable, i);
					else
						checkFuncBody(lextable, idtable, i);

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
				reserve++;
				if(!checkExpression(lextable, idtable, i, IT::UNDEF, true))
					throw ERROR_THROW_LINE(711, entry.sn);
				checkSwitch(lextable, idtable, i);
				break;


			case LEX_BREAK:
				if(!reserve) throw ERROR_THROW_LINE(721, entry.sn);
			}
		}
		return true;
	}
}
