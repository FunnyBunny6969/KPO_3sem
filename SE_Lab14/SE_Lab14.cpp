#include "stdafx.h"
#include <iostream>
#include <locale>

#include <cwchar>
#include <tchar.h>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Out.h"

using namespace std;


void geterrorTEST();
void getparmTEST(int, _TCHAR*[]);
void getinTEST(int, _TCHAR*[]);
void getlogTEST(int, _TCHAR*[]);
void outTEST(int, _TCHAR*[]);




int _tmain(int argc, _TCHAR* argv[])
{
	//geterrorTEST();
	//getparmTEST(argc, argv);
	getinTEST(argc, argv);
	//getlogTEST(argc, argv);
	//outTEST(argc, argv);
	return 0;
}




void geterrorTEST() {
	setlocale(LC_ALL, "rus");
	std::cout << "--- тест Error::geterror ---" << std::endl << std::endl;

	try { throw ERROR_THROW(100); }
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
	};

	std::cout << "-- тест Error::geterrorin ---" << std::endl << std::endl;
	try { throw ERROR_THROW_IN(111, 7, 7); }
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message
			<< ", строка " << e.inext.line
			<< ", позиция " << e.inext.col << std::endl << std::endl;
		system("pause");
	}
}


void getparmTEST(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");
	std::cout << "---- Тест Parm::getparm ----" << std::endl;

	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		std::wcout << "-in:" << parm.in << 
			", -out:" << parm.out << 
			", -log:" << parm.log << std::endl << std::endl;
	}

	catch (Error::ERROR e) {
		std::cout << 
			"Ошибка: " << e.id << ": " << e.message << 
			std::endl << std::endl;
	}

	system("pause");
}

void getinTEST(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");
	std::cout << "---- тест In::getin  --- " << std::endl << std::endl;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		std::cout << in.text << std::endl;
		std::cout << "Всего символов: " << in.size << std::endl;
		std::cout << "Всего строк: " << in.lines << std::endl;
		std::cout << "Пропущено: " << in.ignor << std::endl;
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;
	};
	system("pause");
}



void getlogTEST(int argc, _TCHAR* argv[]) {
	Log::LOG log = Log::INITLOG;

	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);

		Log::WriteLine(log, (char*)"Тест", (char*)" без ошибок \n", (char*)"");
		Log::WriteLine(log, (wchar_t*)L"Тест", (wchar_t*)L" без ошибок \n", (wchar_t*)L"");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);

		Log::Close(log);
	}
	catch (const Error::ERROR& e) {
		Log::WriteError(log, e);
	}
}

void outTEST(int argc, _TCHAR* argv[]) {
	Out::OUT out = Out::INITOUT;

	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		out = Out::getout(parm.out);
		In::IN in = In::getin(parm.in);
		Out::WriteInOut(out, in);
	}
	catch (Error::ERROR e)
	{
		Out::WriteErrorOut(out, e);
	};
}

