#pragma once
#include "AST.h"
#include "IT.h"
#include "Out.h"
#include <string>
#include <vector>
#include <memory>

namespace CodeGen {

    class Generator {
    public:
        Generator(const IT::IdTable& it, Out::OUT& out);
        void Generate(const AST::ProgramNode* program);

    private:
        const IT::IdTable& it_;
        Out::OUT& out_;
        int labelCounter_ = 0;

        std::string NewLabel(const std::string& prefix);
        std::string GetMasmVarName(int idxTI) const;
        std::string GetMasmFuncName(int idxTI) const;

        void EmitHeader();
        void EmitDataSection(const AST::ProgramNode* program);
        void EmitCodeSection(const AST::ProgramNode* program);

        void EmitFunction(const AST::FunctionNode* func);
        void EmitBlock(const std::vector<std::unique_ptr<AST::StmtNode>>& stmts, const std::string& exitLabel);
        void EmitStatement(const AST::StmtNode* stmt, const std::string& exitLabel);

        void EmitExpr(const AST::ExprNode* expr);
    };

    void Run(const AST::ProgramNode* program, const IT::IdTable& it, Out::OUT& out);

} // namespace CodeGen
