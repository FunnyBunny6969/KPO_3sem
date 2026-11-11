#include "Log.h"
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


    IT::Entry CreateITEntry(const char* id, int lexemeIndex, IT::IDDATATYPE dataType, IT::IDTYPE type) {
        IT::Entry entry;
        strcpy_s(entry.id, ID_MAXSIZE, id);
        entry.idxfirstLE = lexemeIndex;
        entry.iddatatype = dataType;
        entry.idtype = type;
        entry.value.vint = TI_INT_DEFAULT;
        return entry;
    }

	void TestLT(Log::LOG log) {
		std::cout << "=== ТЕСТ ТАБЛИЦЫ ЛЕКСЕМ ===" << std::endl;

		LT::LexTable lt = LT::Create(10);

		LT::Add(lt, CreateLTEntry('t', 1, LT_TI_NULLIDX)); // integer
		LT::Add(lt, CreateLTEntry('i', 1, 0));             // идентификатор
		LT::Add(lt, CreateLTEntry('f', 1, LT_TI_NULLIDX)); // function
		LT::Add(lt, CreateLTEntry('(', 1, LT_TI_NULLIDX)); // (
		LT::Add(lt, CreateLTEntry(')', 1, LT_TI_NULLIDX)); // )
		LT::Add(lt, CreateLTEntry('{', 2, LT_TI_NULLIDX)); // {

		std::cout << "Размер таблицы: " << lt.size << std::endl;

		PrintLexTable(lt, log);

		LT::Delete(lt);
	}


    void PrintLexTable(LT::LexTable& lextable, Log::LOG log) {
        if (lextable.size == 0) {
            *log.stream << "Таблица лексем пуста" << endl;
            return;
        }

        *log.stream << "----- ТАБЛИЦА ЛЕКСЕМ -----" << endl;

        int currentLine = 1;
        bool firstOnLine = true;

        for (int i = 0; i < lextable.size; i++) {
            LT::Entry entry = lextable.table[i];

            if (entry.sn != currentLine) {
                *log.stream << std::endl;
                currentLine = entry.sn;
                firstOnLine = true;
            }

            if (firstOnLine) {
                if (currentLine < 10) *log.stream << "0";
                *log.stream << currentLine << " ";
                firstOnLine = false;
            }

            *log.stream << entry.lexema[0];
        }
        *log.stream << std::endl;
    }

	void PrintLexTableWithSubstit(LT::LexTable& lextable, IT::IdTable& idtable, Log::LOG log)
	{
		*log.stream << "\n----- ТАБЛИЦА ЛЕКСЕМ (с именами) -----\n";

		int currentLine = 1;
		bool firstOnLine = true;

		for (int i = 0; i < lextable.size; i++) {
			LT::Entry entry = lextable.table[i];

			if (entry.sn != currentLine) {
				*log.stream << std::endl;
				currentLine = entry.sn;
				firstOnLine = true;
			}

			if (firstOnLine) {
				if (currentLine < 10) *log.stream << "0";
				*log.stream << currentLine << " ";
				firstOnLine = false;
			}



			if (entry.lexema[0] == LEX_ID) {
				IT::Entry e = idtable.table[entry.idxTI];

				if (e.idtype == IT::F) {
					*log.stream << e.id
						<< "@" << e.func_meta.n_params << " ";
				}

				else if (e.idtype == IT::V || e.idtype == IT::P) {
					*log.stream << entry.lexema[0] << e.id << " ";
				}

				else {
					*log.stream << "ID_UNKNOWN_TYPE ";
				}
			}

			else if (entry.lexema[0] == LEX_LITERAL) {
				IT::Entry e = idtable.table[entry.idxTI];
				if (e.iddatatype == IT::UINT)
					*log.stream << e.value.vint << " ";
				else if (e.iddatatype == IT::CHAR)
					*log.stream << "'" << e.value.vchar << "' ";
				else if (e.iddatatype == IT::STR)
					*log.stream << "'" << e.value.vstr->str << "' ";
				else {
					*log.stream << "LITERAL_UNKNOWN_TYPE ";
				}
			}

			else {
				*log.stream << entry.lexema[0] << " ";
			}
		}
		*log.stream << "\n----------------------------------------\n";
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

    void PrintTable(IT::IdTable& idtable, Log::LOG log) {
        if (idtable.size == 0) {
            *log.stream << "Таблица идентификаторов пуста" << std::endl;
            return;
        }

        *log.stream << "IT (Identifier Table): size=" << idtable.size << std::endl;
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), "%-6s %-15s %-8s %-8s %-12s %-15s %s\n",
            "Index", "Identifier", "Type", "DataType", "FirstLT", "Scope", "Value");
        *log.stream << buffer;
        *log.stream << "--------------------------------------------------------------------" << std::endl;

        for (int i = 0; i < idtable.size; i++) {
            IT::Entry entry = idtable.table[i];

            const char* dataTypeStr = "";
            switch (entry.iddatatype) {
            case IT::UINT:   dataTypeStr = "UINT"; break;
            case IT::STR:    dataTypeStr = "STR"; break;
            case IT::CHAR:   dataTypeStr = "CHAR";break;
            case IT::UNDEF:  dataTypeStr = "UNDEF";break;
            }

            const char* typeStr = "";
            switch (entry.idtype) {
            case IT::V: typeStr = "VAR"; break;
            case IT::F: typeStr = "FUNC"; break;
            case IT::P: typeStr = "PARM"; break;
            case IT::L: typeStr = "LIT"; break;
            }

            // Формируем значение для вывода
            string valueStr = "";
            if (entry.idtype == IT::L) {
                if (entry.iddatatype == IT::UINT) {
                    valueStr = to_string(entry.value.vint);
                }
                else if (entry.iddatatype == IT::CHAR) {
                    valueStr = "'";
                    valueStr += entry.value.vchar;
                    valueStr += "'";
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



            string SCOPE = "=EmptY=";
            if (entry.scope == GLOBAL_SCOPE) {
				SCOPE = "GLOBAL";
            }
            else if (entry.scope == MAIN_SCOPE) {
				SCOPE = "MAIN";
            }
            else if (entry.scope == EMPTY_SCOPE) {
				SCOPE = "=EmptY=";
            }
            else {
                SCOPE = idtable.table[entry.scope].id;
            }

            snprintf(buffer, sizeof(buffer), "%-6d %-15s %-8s %-8s %-12d %-15s %s\n",
                i, entry.id, typeStr, dataTypeStr, entry.idxfirstLE, SCOPE.c_str(), valueStr.c_str());
			*log.stream << buffer;
        }
    }

    void TestIT(Log::LOG log) {
        std::cout << "\n=== ТЕСТ ТАБЛИЦЫ ИДЕНТИФИКАТОРОВ ===" << std::endl;

        IT::IdTable it = IT::Create(10);

        IT::Add(it, CreateEntry("main", 0, IT::UINT, IT::F));    // функция
        IT::Add(it, CreateEntry("x", 2, IT::UINT, IT::P));       // параметр
        IT::Add(it, CreateEntry("y", 3, IT::UINT, IT::P));       // параметр
        IT::Add(it, CreateEntry("z", 4, IT::UINT, IT::V));       // переменная

        // Добавим тестовые литералы
        IT::Entry litEntry;
        strcpy_s(litEntry.id, ID_MAXSIZE, "L1");
        litEntry.idxfirstLE = 5;
        litEntry.iddatatype = IT::UINT;
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

        PrintTable(it, log);

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
        TestSingleAutomata(Automata::U_INTEGER, "unsigned_integer", true, "INTEGER");
        TestSingleAutomata(Automata::U_INTEGER, "intege", false, "INTEGER");
        TestSingleAutomata(Automata::U_INTEGER, "int", false, "INTEGER");
        TestSingleAutomata(Automata::U_INTEGER, "INTEGER", false, "INTEGER");

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
            {"integer", LEX_UINT, "ключевое слово integer"},
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
    void TestSplitter(In::IN in, Log::LOG log) {
		std::vector<std::vector<std::string>> lines;
		LEX::SplitIntoWords((const char*)in.text, lines);
		*log.stream << "=== РЕЗУЛЬТАТ РАЗБИВКИ ===" << std::endl;

		for (int i = 0; i < lines.size(); i++) {
			*log.stream << "Строка " << (i + 1) << " (" << lines[i].size() << " слов): ";

			for (int j = 0; j < lines[i].size(); j++) {
				*log.stream << "[" << lines[i][j] << "] ";
			}
			*log.stream << std::endl;
		}
		*log.stream << "Всего строк: " << lines.size() << std::endl;
    }


    void PrintFunctionParameters(const IT::IdTable& idTable, Log::LOG log)
    {
        // Заголовок для вывода
        *log.stream << "\n--- FUNCTION METADATA DUMP ---\n";
        char buffer[1024];
		snprintf(buffer, sizeof(buffer), "%-32s | %s | %s\n", "FUNCTION NAME", "PARAMS COUNT", "PARAM TYPES");
        *log.stream << buffer;
        *log.stream << "---------------------------------+--------------+----------------------------\n";

        // Итерация по Таблице Идентификаторов
        for (int i = 0; i < idTable.size; i++) {
            const IT::Entry& currentId = idTable.table[i];

            // Ищем только записи функций
            if (currentId.idtype == IT::F) {

                // Имя функции
                const char* funcName = currentId.id;
                // Количество параметров
                int nParams = currentId.func_meta.n_params;

                // Формируем строку с типами параметров
                char paramTypesStr[100] = { 0 }; // Буфер для типов
                char* ptr = paramTypesStr;

                for (int p = 0; p < nParams; p++) {
                    IT::IDDATATYPE type = currentId.func_meta.params_types[p];
                    const char* typeStr = "UNKNOWN";

                    // Конвертируем тип в строку (вам нужно будет адаптировать эти константы)
                    if (type == IT::UINT) {       typeStr = "INT"; }
                    else if (type == IT::STR) {  typeStr = "STRING"; }
                    else if (type == IT::CHAR) { typeStr = "CHAR"; } 

                    // Копируем строку типа
                    int len = snprintf(ptr, 100 - (ptr - paramTypesStr), "%s", typeStr);
                    ptr += len;

                    // Добавляем разделитель, если это не последний параметр
                    if (p < nParams - 1) {
                        len = snprintf(ptr, 100 - (ptr - paramTypesStr), ", ");
                        ptr += len;
                    }
                }

                // Вывод строки
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), "%-32s | %12d | %s\n", funcName, nParams, paramTypesStr);
				*log.stream << buffer;
            }
        }
        *log.stream << "---------------------------------+--------------+----------------------------\n";
    }
}



