#include "stdafx.h"
#include "Tests.h"
#include "LEX.h"
#include "MFST.h"
#include "PN.h"
#include "Semantic.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;







void run(int argc, _TCHAR* argv[]) {
	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INITOUT;
	In::IN in;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		in = In::getin(parm.in);
		log = Log::getlog(parm.log);
		out = Out::getout(parm.out);

		Out::WriteInOut(out, in);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);


        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;
		LEX::LEX tables; 
		IT::InitBuiltins(tables.idTable);
        LEX::Analyze((const char*)in.text, tables.lexTable, tables.idTable);


		//==================================
		MFST_RUN
		//==================================

        TestLT::PrintLexTable(tables.lexTable);
        TestIT::PrintTable(tables.idTable);
		TestLexer::PrintFunctionParameters(tables.idTable);



		if (SemanticAnalyzer::RunSemanter(tables.lexTable, tables.idTable))
			cout << "СЕМАНТИЧЕСКИЙ АНАЛИЗ ВЫПОЛНЕН БЕЗ ОШИБОК" << endl;





		//==================================
		//PN::FindExpressions(tables.lexTable, tables.idTable);
		//==================================


        //TestLT::PrintLexTable(tables.lexTable);
		//TestLT::PrintLexTableWithSubstit(tables.lexTable, tables.idTable);



        // Очищаем память
        LT::Delete(tables.lexTable);
        IT::Delete(tables.idTable);
        Log::Close(log);
		Out::Close(out);
    }
	catch (const Error::ERROR& e) {
		cout << "Ошибка " << e.id << " : " << e.message 
			<< " LINE " << e.inext.line 
			<< " COL "  << e.inext.col << endl << endl;

		Log::WriteError(log, e);
		Out::WriteErrorOut(out, e);
        Log::Close(log);
		Out::Close(out);
	}
	catch (const std::exception& e) {
		cout << "НЕИЗВЕСТНАЯ ОШИБКА, ps возможно упал синтаксический анализатор" << endl;
	}



	/*
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		out = Out::getout(parm.out);
		In::IN in = In::getin(parm.in);
		Out::WriteInOut(out, in);
		Out::Close(out);
	}
	catch (Error::ERROR e)
	{
		cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;
		Out::WriteErrorOut(out, e);
	};
	*/
}



int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "Russian");
	run(argc, argv);
	return 0;
}
