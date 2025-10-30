#include "stdafx.h"
#include "Tests.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "Russian");
	TestLT::TestLT();
	TestIT::TestIT();

	


/*
	Log::LOG log = Log::INITLOG;
	try {
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
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
		std::cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;
		Out::WriteErrorOut(out, e);
	};

	*/

	return 0;
}
