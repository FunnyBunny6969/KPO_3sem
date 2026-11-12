#include "stdafx.h"
#include "Tests.h"
#include "LEX.h"
#include "MFST.h"
#include "ASTBuilder.h"
#include "Sem.h"
#include "CodeGen.h"

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <memory>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_CTYPE, "Russian");

    Log::LOG log = Log::INITLOG;
    Out::OUT out = Out::INITOUT;
    In::IN in{};
    in.text = nullptr;

    LEX::LEX tables{};
    bool tablesInitialized = false;

    auto cleanup = [&]() {
        if (tablesInitialized) {
            LT::Delete(tables.lexTable);
            IT::Delete(tables.idTable);
            tablesInitialized = false;
        }
        if (in.text != nullptr) {
            delete[] in.text;
            in.text = nullptr;
        }
        if (log.stream != nullptr) {
            Log::Close(log);
            log.stream = nullptr;
        }
        if (out.streamOut != nullptr) {
            Out::Close(out);
            out.streamOut = nullptr;
        }
        };

    try {
        // Разбор параметров командной строки (-in, -out, -log)
        Parm::PARM parm = Parm::getparm(argc, argv);

        // Открываем файлы протокола и вывода до чтения исходника
        log = Log::getlog(parm.log);
        out = Out::getout(parm.out);

        Log::WriteLog(log);
        Log::WriteParm(log, parm);

        // Чтение и предварительная очистка исходного файла
        in = In::getin(parm.in);
        Log::WriteIn(log, in);

        // Лексический анализ и построение таблиц LT и IT
        cout << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;
        *log.stream << "\n=== ЛЕКСИЧЕСКИЙ АНАЛИЗ ===" << endl;

        tables.lexTable = LT::Create(LT_MAXSIZE);
        tables.idTable = IT::Create(TI_MAXSIZE);
        tablesInitialized = true;

        IT::InitBuiltins(tables.idTable);
        TestLexer::TestSplitter(in, log);
        LEX::Analyze((const char*)in.text, tables.lexTable, tables.idTable);

        TestLT::PrintLexTable(tables.lexTable, log);
        TestIT::PrintTable(tables.idTable, log);
        TestLexer::PrintFunctionParameters(tables.idTable, log);

        // Синтаксический анализ (магазинный автомат MFST)
        cout << "\n=== СИНТАКСИЧЕСКИЙ АНАЛИЗ (MFST) ===" << endl;
        *log.stream << "\n=== СИНТАКСИЧЕСКИЙ АНАЛИЗ (MFST) ===" << endl;

        MFST::Mfst mfst(tables, GRB::getGreibach(), &log);
        if (!mfst.start()) {
            throw ERROR_THROW(600);
        }
        mfst.savededucation();
        mfst.printrules();

        cout << "Синтаксический анализ выполнен без ошибок." << endl;

        // Построение абстрактного синтаксического дерева (AST)
        cout << "\n=== ПОСТРОЕНИЕ AST ===" << endl;
        *log.stream << "\n=== ПОСТРОЕНИЕ AST ===" << endl;

        AST::Builder astBuilder(tables.lexTable, tables.idTable);
        std::unique_ptr<AST::ProgramNode> ast = astBuilder.Build();

        AST::PrintAST(ast.get(), tables.idTable, std::cout);
        AST::PrintAST(ast.get(), tables.idTable, *log.stream);

        // Семантический анализ по AST
        cout << "\n=== СЕМАНТИЧЕСКИЙ АНАЛИЗ ===" << endl;
        *log.stream << "\n=== СЕМАНТИЧЕСКИЙ АНАЛИЗ ===" << endl;

        if (Semantic::Run(ast.get(), tables.idTable)) {
            cout << "Семантический анализ выполнен без ошибок." << endl;
            *log.stream << "Семантический анализ выполнен без ошибок." << endl;
        }

        // Генерация ассемблерного кода MASM
        cout << "\n=== ГЕНЕРАЦИЯ КОДА (MASM) ===" << endl;
        *log.stream << "\n=== ГЕНЕРАЦИЯ КОДА (MASM) ===" << endl;

        CodeGen::Run(ast.get(), tables.idTable, out);

        cout << "Ассемблерный код успешно сгенерирован." << endl;
        *log.stream << "Ассемблерный код успешно сгенерирован." << endl;

        cout << "\n=== КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО ===" << endl;
        *log.stream << "\n=== КОМПИЛЯЦИЯ ЗАВЕРШЕНА УСПЕШНО ===" << endl;

        cleanup();
    }
    catch (const Error::ERROR& e) {
        cout << "\nERROR " << e.id << " : " << e.message;
        if (e.inext.line != -1) {
            cout << " (строка " << e.inext.line;
            if (e.inext.col != -1) cout << ", позиция " << e.inext.col;
            cout << ")";
        }
        cout << endl << endl;

        Log::WriteError(log, e);
        Out::WriteErrorOut(out, e);

        cleanup();
        return e.id;
    }
    catch (const std::exception& e) {
        cout << "\nERROR: Системное исключение: " << e.what() << endl;
        if (log.stream != nullptr && log.stream->is_open()) {
            *log.stream << "\nERROR: Системное исключение: " << e.what() << endl;
        }
        cleanup();
        return -1;
    }

    return 0;
}
