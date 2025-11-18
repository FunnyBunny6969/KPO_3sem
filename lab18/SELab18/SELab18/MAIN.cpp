#include "stdafx.h"
#include "Tests.h"
#include "LEX.h"
#include <iostream>
using namespace std;



void run(int argc, _TCHAR* argv[]) {
	Log::LOG log = Log::INITLOG;
	In::IN in;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		in = In::getin(parm.in);
		cout << "0";
		log = Log::getlog(parm.log);
		cout << "1";
		Log::WriteLog(log);
		cout << "2";
		Log::WriteParm(log, parm);
		cout << "3";
		Log::WriteIn(log, in);
		cout << "4";
		Log::Close(log);


        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;
        // Создаем таблицы
		LEX::LEX tables; 

        // Запускаем лексический анализ
        LEX::Analyze((const char*)in.text, tables.lexTable, tables.idTable);

        // Выводим результаты
        TestLT::PrintLexTable(tables.lexTable);
        TestIT::PrintTable(tables.idTable);
		TestLexer::TestSplitter(in);

        // Очищаем память
        LT::Delete(tables.lexTable);
		cout << "h";
        IT::Delete(tables.idTable);
		cout << "u";
        Log::Close(log);
		cout << "i";
		cout << "THE END ITS ME====" << endl;
    }
	catch (const Error::ERROR& e) {
		std::cout << "Ошибка " << e.id << " : " << e.message 
			<< " LINE " << e.inext.line 
			<< " COL "  << e.inext.col << std::endl << std::endl;
		Log::WriteError(log, e);

		cout << "ebat its me";

		if (e.id != 111) {
			TestLexer::TestSplitter(in);
		}
	}

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
