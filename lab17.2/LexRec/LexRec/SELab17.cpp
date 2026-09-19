#include "stdafx.h"
#include "Tests.h"
#include "Lexer.h"
#include <iostream>
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

		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		Out::WriteInOut(out, in);
		Log::WriteIn(log, in);



        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;
        // Создаем таблицы
        LT::LexTable lexTable = LT::Create(1000);
        IT::IdTable idTable = IT::Create(100);


        // Запускаем лексический анализ
        Lexer::Analyze((const char*)in.text, lexTable, idTable);

        // Выводим результаты
        TestLT::PrintLexTable(lexTable);
        TestIT::PrintTable(idTable);
		TestLexer::TestSplitter(in);

        // Очищаем память
        LT::Delete(lexTable);
        IT::Delete(idTable);
        Log::Close(log);
    }
	catch (const Error::ERROR& e) {
		std::cout << "Ошибка " << e.id << " : " << e.message 
			<< " LINE " << e.inext.line 
			<< " COL "  << e.inext.col << std::endl << std::endl;
		if (e.id >= 200) {
			TestLexer::TestSplitter(in);
		}

		Log::WriteError(log, e);
		Out::WriteErrorOut(out, e);
	}

	Out::Close(out);
	Log::Close(log);

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
