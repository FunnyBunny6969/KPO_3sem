#pragma once
#include "AST.h"
#include "IT.h"

namespace Semantic {

    class Analyzer {
    public:
        explicit Analyzer(IT::IdTable& it);
        bool Analyze(AST::ProgramNode* program);

    private:
        IT::IdTable& it_;

        // Контекст текущей анализируемой функции
        IT::IDDATATYPE currentReturnType_ = IT::UNDEF;
        bool isCurrentMain_ = false;
        bool hasReturnStmt_ = false;
        int whileDepth_ = 0;

        // Обход узлов AST
        void AnalyzeFunction(AST::FunctionNode* func);
        void AnalyzeBlock(const std::vector<std::unique_ptr<AST::StmtNode>>& stmts);
        void AnalyzeStatement(AST::StmtNode* stmt);

        // Вычисление и проверка типа выражения (заполняет expr->evalType)
        IT::IDDATATYPE AnalyzeExpr(AST::ExprNode* expr);

        // Вспомогательная проверка на константный 0 (для отлова деления на ноль 714)
        bool IsZeroConstant(const AST::ExprNode* expr) const;
    };

    // Функция-обертка для вызова из main.cpp
    bool Run(AST::ProgramNode* program, IT::IdTable& it);

} // namespace Semantic
