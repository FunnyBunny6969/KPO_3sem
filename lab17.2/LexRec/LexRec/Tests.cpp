#include "Tests.h"
#include <iostream>
using namespace std;


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
		std::cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;
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


		Error::ERROR e = Error::geterrorin(112, 2, 3);
		Out::WriteErrorOut(out, e);

		Out::Close(out);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << " : " << e.message << std::endl << std::endl;
		Out::WriteErrorOut(out, e);
	};
}


// Вспомогательные функции для тестирования LT
namespace TestLT {
    // Создать лексему для добавления в таблицу
    LT::Entry CreateLTEntry(char lexema, int lineNumber, int tiIndex) {
        LT::Entry entry;
        entry.lexema[0] = lexema;
        entry.lexema[1] = '\0';
        entry.sn = lineNumber;
        entry.idxTI = tiIndex;
        return entry;
    }

    // Вывести таблицу лексем в формате из задания
    void PrintLexTable(LT::LexTable& lextable) {
        if (lextable.size == 0) {
            cout << "Таблица лексем пуста" << std::endl;
            return;
        }

        cout << "----- ТАБЛИЦА ЛЕКСЕМ -----" << std::endl;

        int currentLine = 1;
        bool firstOnLine = true;

        for (int i = 0; i < lextable.size; i++) {
            LT::Entry entry = lextable.table[i];

            if (entry.sn != currentLine) {
                std::cout << std::endl;
                currentLine = entry.sn;
                firstOnLine = true;
            }

            if (firstOnLine) {
                if (currentLine < 10) std::cout << "0";
                std::cout << currentLine << " ";
                firstOnLine = false;
            }

            std::cout << entry.lexema[0];
        }
        std::cout << std::endl;
    }

    // Вспомогательные для IT...
    IT::Entry CreateITEntry(const char* id, int lexemeIndex, IT::IDDATATYPE dataType, IT::IDTYPE type) {
        IT::Entry entry;
        strcpy_s(entry.id, ID_MAXSIZE, id);
        entry.idxfirstLE = lexemeIndex;
        entry.iddatatype = dataType;
        entry.idtype = type;
        entry.value.vint = TI_INT_DEFAULT;
        return entry;
    }

	void TestLT() {
		std::cout << "=== ТЕСТ ТАБЛИЦЫ ЛЕКСЕМ ===" << std::endl;

		LT::LexTable lt = LT::Create(10);

		// Добавляем лексемы используя функции из TestHelpers
		LT::Add(lt, CreateLTEntry('t', 1, LT_TI_NULLIDX)); // integer
		LT::Add(lt, CreateLTEntry('i', 1, 0));             // идентификатор
		LT::Add(lt, CreateLTEntry('f', 1, LT_TI_NULLIDX)); // function
		LT::Add(lt, CreateLTEntry('(', 1, LT_TI_NULLIDX)); // (
		LT::Add(lt, CreateLTEntry(')', 1, LT_TI_NULLIDX)); // )
		LT::Add(lt, CreateLTEntry('{', 2, LT_TI_NULLIDX)); // {

		std::cout << "Размер таблицы: " << lt.size << std::endl;

		// Выводим используя функцию из TestHelpers
		PrintLexTable(lt);

		LT::Delete(lt);
	}
}


// Вспомогательные функции для тестирования IT
namespace TestIT {
    // Создать запись идентификатора
    IT::Entry CreateEntry(const char* id, int lexemeIndex, IT::IDDATATYPE dataType, IT::IDTYPE type) {
        IT::Entry entry;
        strcpy_s(entry.id, ID_MAXSIZE, id);  // Убрал IT::
        entry.idxfirstLE = lexemeIndex;
        entry.iddatatype = dataType;
        entry.idtype = type;
        entry.value.vint = TI_INT_DEFAULT;   // Убрал IT::
        return entry;
    }

    // Вывести таблицу идентификаторов
    void PrintTable(IT::IdTable& idtable) {
        if (idtable.size == 0) {
            std::cout << "Таблица идентификаторов пуста" << std::endl;
            return;
        }

        std::cout << "----- ТАБЛИЦА ИДЕНТИФИКАТОРОВ -----" << std::endl;
        std::cout << "Индекс | Идентификатор | Тип данных | Тип | Первое вхождение" << std::endl;
        std::cout << "-----------------------------------------------------------" << std::endl;

        for (int i = 0; i < idtable.size; i++) {
            IT::Entry entry = idtable.table[i];

            std::cout << i << "      | " << entry.id << "           | ";

            // Тип данных
            if (entry.iddatatype == IT::INT) {
                std::cout << "integer   | ";
            }
            else {
                std::cout << "string    | ";
            }

            // Тип идентификатора
            switch (entry.idtype) {
            case IT::V: std::cout << "переменная | "; break;
            case IT::F: std::cout << "функция    | "; break;
            case IT::P: std::cout << "параметр   | "; break;
            case IT::L: std::cout << "литерал    | "; break;
            }

            // Первое вхождение
            std::cout << entry.idxfirstLE << std::endl;
        }
    }

    void TestIT() {
        std::cout << "\n=== ТЕСТ ТАБЛИЦЫ ИДЕНТИФИКАТОРОВ ===" << std::endl;

        IT::IdTable it = IT::Create(10);

        // Добавляем идентификаторы
        IT::Add(it, CreateEntry("main", 0, IT::INT, IT::F));    // функция
        IT::Add(it, CreateEntry("x", 2, IT::INT, IT::P));       // параметр
        IT::Add(it, CreateEntry("y", 3, IT::INT, IT::P));       // параметр
        IT::Add(it, CreateEntry("z", 4, IT::INT, IT::V));       // переменная

        std::cout << "Размер таблицы: " << it.size << std::endl;

        // Самый простой вариант
        char mainId[] = "main";
        int index = IT::IsId(it, mainId);
        std::cout << "Идентификатор 'main' найден по индексу: " << index << std::endl;

        // Выводим таблицу
        PrintTable(it);

        IT::Delete(it);
    }
}
