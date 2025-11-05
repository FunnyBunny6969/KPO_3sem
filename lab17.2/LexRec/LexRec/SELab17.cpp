#include "stdafx.h"
#include "Tests.h"
#include "Lexer.h"
#include <iostream>
using namespace std;



void run(int argc, _TCHAR* argv[]) {
	Log::LOG log = Log::INITLOG;
	In::IN in;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::Close(log);


        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;
        // Создаем таблицы
        LT::LexTable lexTable = LT::Create(1000);
        IT::IdTable idTable = IT::Create(100);


        // Запускаем лексический анализ
        Lexer::Analyze((const char*)in.text, lexTable, idTable);

        // Выводим результаты
        TestLT::PrintLexTable(lexTable);
        TestIT::PrintTable(idTable);

        // Очищаем память
        LT::Delete(lexTable);
        IT::Delete(idTable);
        Log::Close(log);
    }
	catch (const Error::ERROR& e) {
		std::cout << "Ошибка " << e.id << " : " << e.message 
			<< " LINE " << e.inext.line 
			<< " COL "  << e.inext.col << std::endl << std::endl;
		TestLexer::TestSplitter(in);
		Log::WriteError(log, e);
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
	//TestIT::TestIT();
	//TestLT::TestLT();
	//TestAutomata::RunAllTests();




	return 0;
}
