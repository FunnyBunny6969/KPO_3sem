#include "stdafx.h"
#include "Tests.h"
#include "Lexer.h"
#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "Russian");

	Log::LOG log = Log::INITLOG;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::Close(log);


        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;

        // Создаем таблицы
        LT::LexTable lexTable = LT::Create(1000);
        IT::IdTable idTable = IT::Create(100);
        std::vector<Error::ERROR> lexErrors;

        // Запускаем лексический анализ
		Lexer::Analyze((const char*)in.text, lexTable, idTable, lexErrors);

        // Выводим результаты
        TestLT::PrintLexTable(lexTable);
        TestIT::PrintTable(idTable);

        // Выводим ошибки лексического анализа
        if (!lexErrors.empty()) {
            cout << "\n=== ОШИБКИ ЛЕКСИЧЕСКОГО АНАЛИЗА ===" << endl;
            for (const auto& error : lexErrors) {
                cout << "Строка " << error.inext.line << ", позиция " << error.inext.col
                    << ": " << error.message << endl;
            }
        }

        // Очищаем память
        LT::Delete(lexTable);
        IT::Delete(idTable);
        // === КОНЕЦ ЛЕКСЕРА ===

        Log::Close(log);
    }
	catch (const Error::ERROR& e) {
		std::cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;
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




	return 0;
}
