#pragma once
#include "AST.h"
#include "LT.h"
#include "IT.h"
#include <memory>

namespace AST {

    class Builder {
    public:
        Builder(const LT::LexTable& lt, const IT::IdTable& it);
        std::unique_ptr<ProgramNode> Build();

    private:
        const LT::LexTable& lt_;
        const IT::IdTable& it_;
        int pos_ = 0;

        // Вспомогательные методы навигации по таблице лексем LT
        bool AtEnd() const;
        const LT::Entry& Current() const;
        const LT::Entry& Peek(int offset = 1) const;
        char CurrentCode() const;
        void Advance();

        // Разбор верхнего уровня и операторов
        std::unique_ptr<FunctionNode> ParseFunction();
        std::unique_ptr<FunctionNode> ParseMain();
        std::vector<std::unique_ptr<StmtNode>> ParseBlock();
        std::unique_ptr<StmtNode> ParseStatement();

        // Разбор выражений с учетом приоритетов (от низшего к высшему)
        std::unique_ptr<ExprNode> ParseExpression();
        std::unique_ptr<ExprNode> ParseComparison();
        std::unique_ptr<ExprNode> ParseAddSubOr();
        std::unique_ptr<ExprNode> ParseMulDivAnd();
        std::unique_ptr<ExprNode> ParseUnary();
        std::unique_ptr<ExprNode> ParsePrimary();
    };


    void PrintAST(const ProgramNode* program, const IT::IdTable& it, std::ostream& os);
} // namespace AST
