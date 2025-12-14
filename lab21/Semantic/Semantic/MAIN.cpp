#include "stdafx.h"
#include "Tests.h"
#include "LEX.h"
#include "MFST.h"
#include "PN.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
using namespace std;







void run(int argc, _TCHAR* argv[]) {
	Log::LOG log = Log::INITLOG;
	In::IN in;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		in = In::getin(parm.in);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);


        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;
		LEX::LEX tables; 
		IT::InitBuiltins(tables.idTable);
        LEX::Analyze((const char*)in.text, tables.lexTable, tables.idTable);

        // Выводим результаты
        TestLT::PrintLexTable(tables.lexTable);
		//TestLexer::TestSplitter(in);


		//==================================
		//MFST_RUN
		//==================================


		//==================================
		PN::FindExpressions(tables.lexTable, tables.idTable);
		//==================================


		TestLexer::PrintFunctionParameters(tables.idTable);
        TestIT::PrintTable(tables.idTable);
        TestLT::PrintLexTable(tables.lexTable);
		TestLT::PrintLexTableWithSubstit(tables.lexTable, tables.idTable);



        // Очищаем память
        LT::Delete(tables.lexTable);
        IT::Delete(tables.idTable);
        Log::Close(log);
    }
	catch (const Error::ERROR& e) {
		std::cout << "Ошибка " << e.id << " : " << e.message 
			<< " LINE " << e.inext.line 
			<< " COL "  << e.inext.col << std::endl << std::endl;
		Log::WriteError(log, e);

		if (e.id != 111) {
			TestLexer::TestSplitter(in);
		}
	}



	Out::OUT out = Out::INITOUT;
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
}



int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "Russian");
	run(argc, argv);
	return 0;
}
