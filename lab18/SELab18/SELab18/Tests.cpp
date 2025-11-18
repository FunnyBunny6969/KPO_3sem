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
    LT::Entry CreateLTEntry(char lexema, int lineNumber, int tiIndex) {
        LT::Entry entry;
        entry.lexema[0] = lexema;
        entry.lexema[1] = '\0';
        entry.sn = lineNumber;
        entry.idxTI = tiIndex;
        return entry;
    }

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

		LT::Add(lt, CreateLTEntry('t', 1, LT_TI_NULLIDX)); // integer
		LT::Add(lt, CreateLTEntry('i', 1, 0));             // идентификатор
		LT::Add(lt, CreateLTEntry('f', 1, LT_TI_NULLIDX)); // function
		LT::Add(lt, CreateLTEntry('(', 1, LT_TI_NULLIDX)); // (
		LT::Add(lt, CreateLTEntry(')', 1, LT_TI_NULLIDX)); // )
		LT::Add(lt, CreateLTEntry('{', 2, LT_TI_NULLIDX)); // {

		std::cout << "Размер таблицы: " << lt.size << std::endl;

		PrintLexTable(lt);

		LT::Delete(lt);
	}
}

// Вспомогательные функции для тестирования IT
namespace TestIT {
    IT::Entry CreateEntry(const char* id, int lexemeIndex, IT::IDDATATYPE dataType, IT::IDTYPE type) {
        IT::Entry entry;
        strcpy_s(entry.id, ID_MAXSIZE, id);
        entry.idxfirstLE = lexemeIndex;
        entry.iddatatype = dataType;
        entry.idtype = type;
        entry.value.vint = TI_INT_DEFAULT;
        return entry;
    }

    void PrintTable(IT::IdTable& idtable) {
        if (idtable.size == 0) {
            std::cout << "Таблица идентификаторов пуста" << std::endl;
            return;
        }

        std::cout << "IT (Identifier Table): size=" << idtable.size << std::endl;
        printf("%-6s %-15s %-8s %-8s %-12s %s\n",
            "Index", "Identifier", "Type", "DataType", "FirstLT", "Value");
        std::cout << "--------------------------------------------------------------------" << std::endl;

        for (int i = 0; i < idtable.size; i++) {
            IT::Entry entry = idtable.table[i];

            const char* dataTypeStr = (entry.iddatatype == IT::INT) ? "INT" : "STR";
            const char* typeStr = "";
            switch (entry.idtype) {
            case IT::V: typeStr = "VAR"; break;
            case IT::F: typeStr = "FUNC"; break;
            case IT::P: typeStr = "PARM"; break;
            case IT::L: typeStr = "LIT"; break;
            }

            // Формируем значение для вывода
            std::string valueStr = "";
            if (entry.idtype == IT::L) {
                if (entry.iddatatype == IT::INT) {
                    valueStr = std::to_string(entry.value.vint);
                }
                else {
                    valueStr = "'";
                    for (int j = 0; j < entry.value.vstr[0].len; j++) {
                        valueStr += entry.value.vstr[0].str[j];
                    }
                    valueStr += "'";
                }
            }
            else {
                valueStr = "-"; // для не-литералов
            }

            printf("%-6d %-15s %-8s %-8s %-12d %s\n",
                i, entry.id, typeStr, dataTypeStr, entry.idxfirstLE, valueStr.c_str());
        }
    }

    void TestIT() {
        std::cout << "\n=== ТЕСТ ТАБЛИЦЫ ИДЕНТИФИКАТОРОВ ===" << std::endl;

        IT::IdTable it = IT::Create(10);

        IT::Add(it, CreateEntry("main", 0, IT::INT, IT::F));    // функция
        IT::Add(it, CreateEntry("x", 2, IT::INT, IT::P));       // параметр
        IT::Add(it, CreateEntry("y", 3, IT::INT, IT::P));       // параметр
        IT::Add(it, CreateEntry("z", 4, IT::INT, IT::V));       // переменная

        // Добавим тестовые литералы
        IT::Entry litEntry;
        strcpy_s(litEntry.id, ID_MAXSIZE, "L1");
        litEntry.idxfirstLE = 5;
        litEntry.iddatatype = IT::INT;
        litEntry.idtype = IT::L;
        litEntry.value.vint = 123;
        IT::Add(it, litEntry);

        IT::Entry strLitEntry;
        strcpy_s(strLitEntry.id, ID_MAXSIZE, "L2");
        strLitEntry.idxfirstLE = 6;
        strLitEntry.iddatatype = IT::STR;
        strLitEntry.idtype = IT::L;
        strLitEntry.value.vstr[0].len = 11;
        strcpy_s(strLitEntry.value.vstr[0].str, TI_STR_MAXSIZE, "hello world");
        IT::Add(it, strLitEntry);

        std::cout << "Размер таблицы: " << it.size << std::endl;

        char mainId[] = "main";
        int index = IT::IsId(it, mainId);
        std::cout << "Идентификатор 'main' найден по индексу: " << index << std::endl;

        PrintTable(it);

        IT::Delete(it);
    }
}


namespace TestAutomata {

    // Вспомогательная функция для тестирования одного автомата
    void TestSingleAutomata(
        const FST::FST& automata,
        const char* testString,
        bool expected,
        const char* automataName) {

        bool result = Automata::executeAutomata(automata, testString);

        cout << "Тест " << automataName << "(\"" << testString << "\"): ";
        if (result == expected) {
            cout << "ПРОЙДЕН";
        }
        else {
            cout << "НЕ ПРОЙДЕН (ожидалось: " << (expected ? "true" : "false")
                << ", получено: " << (result ? "true" : "false") << ")";
        }
        cout << endl;
    }

    // Тестирование автоматов ключевых слов
    void TestKeywordAutomata() {
        cout << "\n=== ТЕСТИРОВАНИЕ АВТОМАТОВ КЛЮЧЕВЫХ СЛОВ ===" << endl;

        // INTEGER
        TestSingleAutomata(Automata::INTEGER, "integer", true, "INTEGER");
        TestSingleAutomata(Automata::INTEGER, "intege", false, "INTEGER");
        TestSingleAutomata(Automata::INTEGER, "int", false, "INTEGER");
        TestSingleAutomata(Automata::INTEGER, "INTEGER", false, "INTEGER");

        // STRING
        TestSingleAutomata(Automata::STRING, "string", true, "STRING");
        TestSingleAutomata(Automata::STRING, "str", false, "STRING");
        TestSingleAutomata(Automata::STRING, "STRING", false, "STRING");

        // FUNCTION
        TestSingleAutomata(Automata::FUNCTION, "function", true, "FUNCTION");
        TestSingleAutomata(Automata::FUNCTION, "func", false, "FUNCTION");

        // DECLARE
        TestSingleAutomata(Automata::DECLARE, "declare", true, "DECLARE");
        TestSingleAutomata(Automata::DECLARE, "decl", false, "DECLARE");

        // RETURN
        TestSingleAutomata(Automata::RETURN, "return", true, "RETURN");
        TestSingleAutomata(Automata::RETURN, "ret", false, "RETURN");

        // PRINT
        TestSingleAutomata(Automata::PRINT, "print", true, "PRINT");
        TestSingleAutomata(Automata::PRINT, "prin", false, "PRINT");

        // MAIN
        TestSingleAutomata(Automata::MAIN, "main", true, "MAIN");
        TestSingleAutomata(Automata::MAIN, "mai", false, "MAIN");
    }

    // Тестирование автомата идентификаторов
    void TestIdentifierAutomata() {
        cout << "\n=== ТЕСТИРОВАНИЕ АВТОМАТА ИДЕНТИФИКАТОРОВ ===" << endl;

        // Правильные идентификаторы
        TestSingleAutomata(Automata::IDENTIFIER, "x", true, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "variable", true, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "x1", true, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "my_var", true, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "temp123", true, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "a_b_c", true, "IDENTIFIER");

        // Неправильные идентификаторы
        TestSingleAutomata(Automata::IDENTIFIER, "1var", false, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "_var", false, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "my-var", false, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "Var", false, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "test.var", false, "IDENTIFIER");
        TestSingleAutomata(Automata::IDENTIFIER, "", false, "IDENTIFIER");
    }

    // Тестирование автоматов литералов
    void TestLiteralAutomata() {
        cout << "\n=== ТЕСТИРОВАНИЕ АВТОМАТОВ ЛИТЕРАЛОВ ===" << endl;

        // Числовые литералы
        TestSingleAutomata(Automata::NUMBER_LITERAL, "0", true, "NUMBER_LITERAL");
        TestSingleAutomata(Automata::NUMBER_LITERAL, "123", true, "NUMBER_LITERAL");
        TestSingleAutomata(Automata::NUMBER_LITERAL, "999", true, "NUMBER_LITERAL");
        TestSingleAutomata(Automata::NUMBER_LITERAL, "12a", false, "NUMBER_LITERAL");
        TestSingleAutomata(Automata::NUMBER_LITERAL, "", false, "NUMBER_LITERAL");

        // Строковые литералы
        TestSingleAutomata(Automata::STRING_LITERAL, "''", true, "STRING_LITERAL");
        TestSingleAutomata(Automata::STRING_LITERAL, "'hello'", true, "STRING_LITERAL");
        TestSingleAutomata(Automata::STRING_LITERAL, "'ААА   СТРОКА qnjkn124450!@#$%^&*'", true, "STRING_LITERAL");
        TestSingleAutomata(Automata::STRING_LITERAL, "'", false, "STRING_LITERAL");
        TestSingleAutomata(Automata::STRING_LITERAL, "hello", false, "STRING_LITERAL");
    }

    // Тестирование автоматов операторов и разделителей
    void TestOperatorAutomata() {
        cout << "\n=== ТЕСТИРОВАНИЕ АВТОМАТОВ ОПЕРАТОРОВ И РАЗДЕЛИТЕЛЕЙ ===" << endl;

        // Операторы
        TestSingleAutomata(Automata::PLUS, "+", true, "PLUS");
        TestSingleAutomata(Automata::MINUS, "-", true, "MINUS");
        TestSingleAutomata(Automata::STAR, "*", true, "STAR");
        TestSingleAutomata(Automata::SLASH, "/", true, "SLASH");
        TestSingleAutomata(Automata::ASSIGN, "=", true, "ASSIGN");

        // Разделители
        TestSingleAutomata(Automata::SEMICOLON, ";", true, "SEMICOLON");
        TestSingleAutomata(Automata::COMMA, ",", true, "COMMA");
        TestSingleAutomata(Automata::LEFTPAREN, "(", true, "LEFTPAREN");
        TestSingleAutomata(Automata::RIGHTPAREN, ")", true, "RIGHTPAREN");
        TestSingleAutomata(Automata::LEFTBRACE, "{", true, "LEFTBRACE");
        TestSingleAutomata(Automata::RIGHTBRACE, "}", true, "RIGHTBRACE");

        // Неправильные случаи
        TestSingleAutomata(Automata::PLUS, "++", false, "PLUS");
        TestSingleAutomata(Automata::SEMICOLON, ";;", false, "SEMICOLON");
    }

    // Тестирование функции getLexemeCode
    void TestGetLexemeCode() {
        cout << "\n=== ТЕСТИРОВАНИЕ GETLEXEMECODE ===" << endl;

        struct TestCase {
            const char* input;
            char expected;
            const char* description;
        };

        TestCase testCases[] = {
            {"integer", LEX_INTEGER, "ключевое слово integer"},
            {"string", LEX_STRING, "ключевое слово string"},
            {"function", LEX_FUNCTION, "ключевое слово function"},
            {"declare", LEX_DECLARE, "ключевое слово declare"},
            {"return", LEX_RETURN, "ключевое слово return"},
            {"print", LEX_PRINT, "ключевое слово print"},
            {"main", 'm', "ключевое слово main"},
            {"variable", LEX_ID, "идентификатор"},
            {"x1", LEX_ID, "идентификатор с цифрами"},
            {"123", LEX_LITERAL, "числовой литерал"},
            {"'hello'", LEX_LITERAL, "строковый литерал"},
            {"+", LEX_PLUS, "оператор плюс"},
            {"-", LEX_MINUS, "оператор минус"},
            {"*", LEX_STAR, "оператор умножения"},
            {"/", LEX_DIRSLASH, "оператор деления"},
            {"=", '=', "оператор присваивания"},
            {";", LEX_SEMICOLON, "точка с запятой"},
            {",", LEX_COMMA, "запятая"},
            {"(", LEX_LEFTHESIS, "левая скобка"},
            {")", LEX_RIGHTHESIS, "правая скобка"},
            {"{", LEX_LEFTBRACE, "левая фигурная скобка"},
            {"}", LEX_RIGHTBRACE, "правая фигурная скобка"}
        };

        for (const auto& testCase : testCases) {
            char result = Automata::getLexemeCode(testCase.input);
            cout << "getLexemeCode(\"" << testCase.input << "\"): ";

            if (result == testCase.expected) {
                cout << "ПРОЙДЕН (" << testCase.description << " -> '" << result << "')";
            }
            else {
                cout << "НЕ ПРОЙДЕН (ожидалось: '" << testCase.expected
                    << "', получено: '" << result << "')";
            }
            cout << endl;
        }
    }

    // Запуск всех тестов автоматов
    void RunAllTests() {
        cout << "ЗАПУСК ТЕСТОВ АВТОМАТОВ" << endl;
        cout << "=======================" << endl;

        TestKeywordAutomata();
        TestIdentifierAutomata();
        TestLiteralAutomata();
        TestOperatorAutomata();
        TestGetLexemeCode();

        cout << "\n=======================" << endl;
        cout << "ТЕСТИРОВАНИЕ АВТОМАТОВ ЗАВЕРШЕНО" << endl;
    }
}


namespace TestLexer {
    void TestSplitter(In::IN in) {
		std::vector<std::vector<std::string>> lines;
		LEX::SplitIntoWords((const char*)in.text, lines);
		std::cout << "=== РЕЗУЛЬТАТ РАЗБИВКИ ===" << std::endl;

		for (int i = 0; i < lines.size(); i++) {
			std::cout << "Строка " << (i + 1) << " (" << lines[i].size() << " слов): ";

			for (int j = 0; j < lines[i].size(); j++) {
				std::cout << "[" << lines[i][j] << "] ";
			}
			std::cout << std::endl;
		}
		std::cout << "Всего строк: " << lines.size() << std::endl;
    }
}




