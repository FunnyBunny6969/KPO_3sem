#include "ASTBuilder.h"
#include "stdafx.h"
#include <iostream>

namespace AST {

    Builder::Builder(const LT::LexTable& lt, const IT::IdTable& it)
        : lt_(lt), it_(it), pos_(0) {
    }

    bool Builder::AtEnd() const {
        return pos_ >= lt_.size;
    }

    const LT::Entry& Builder::Current() const {
        return lt_.table[pos_];
    }

    const LT::Entry& Builder::Peek(int offset) const {
        int idx = pos_ + offset;
        if (idx < 0 || idx >= lt_.size) {
            return lt_.table[lt_.size - 1];
        }
        return lt_.table[idx];
    }

    char Builder::CurrentCode() const {
        return AtEnd() ? '\0' : lt_.table[pos_].lexema[0];
    }

    void Builder::Advance() {
        if (!AtEnd()) {
            pos_++;
        }
    }

    std::unique_ptr<ProgramNode> Builder::Build() {
        auto program = std::make_unique<ProgramNode>();
        if (lt_.size > 0) {
            program->line = lt_.table[0].sn;
        }

        // идём по таблице лексем и по очереди собираем функции
        while (!AtEnd()) {
            if (CurrentCode() == LEX_MAIN) {
                program->functions.push_back(ParseMain());
            }
            else if (CurrentCode() == LEX_SHORT) {
                program->functions.push_back(ParseFunction());
            }
            else {
                Advance();
            }
        }

        return program;
    }

    std::unique_ptr<FunctionNode> Builder::ParseFunction() {
        auto func = std::make_unique<FunctionNode>();
        func->isMain = false;
        func->line = Current().sn;

        Advance(); // тип ('t')
        Advance(); // 'function' ('f')

        func->funcIdxTI = Current().idxTI;
        Advance(); // имя ('i')

        Advance(); // '('

        while (!AtEnd() && CurrentCode() != LEX_RIGHTHESIS) {
            if (CurrentCode() == LEX_ID) {
                func->paramIdxTI.push_back(Current().idxTI);
            }
            Advance();
        }
        Advance(); // ')'

        func->body = ParseBlock();

        if (CurrentCode() == LEX_SEMICOLON) {
            Advance(); // ';'
        }

        return func;
    }

    std::unique_ptr<FunctionNode> Builder::ParseMain() {
        auto func = std::make_unique<FunctionNode>();
        func->isMain = true;
        func->funcIdxTI = MAIN_SCOPE;
        func->line = Current().sn;

        Advance(); // 'main' ('m')
        func->body = ParseBlock();

        if (CurrentCode() == LEX_SEMICOLON) {
            Advance(); // ';'
        }

        return func;
    }

    std::vector<std::unique_ptr<StmtNode>> Builder::ParseBlock() {
        std::vector<std::unique_ptr<StmtNode>> stmts;
        if (CurrentCode() == LEX_LEFTBRACE) {
            Advance(); // '{'
        }

        while (!AtEnd() && CurrentCode() != LEX_RIGHTBRACE) {
            auto stmt = ParseStatement();
            if (stmt) {
                stmts.push_back(std::move(stmt));
            }
        }

        if (CurrentCode() == LEX_RIGHTBRACE) {
            Advance(); // '}'
        }

        return stmts;
    }

    std::unique_ptr<StmtNode> Builder::ParseStatement() {
        char code = CurrentCode();
        int line = Current().sn;

        // объявление переменной: short a;
        if (code == LEX_SHORT) {
            Advance();
            int varIdx = Current().idxTI;
            Advance();
            if (CurrentCode() == LEX_SEMICOLON) Advance();
            return std::make_unique<VarDeclNode>(varIdx, line);
        }

        // ввод: get a;
        if (code == LEX_GET) {
            Advance();
            int varIdx = Current().idxTI;
            Advance();
            if (CurrentCode() == LEX_SEMICOLON) Advance();
            return std::make_unique<InputNode>(varIdx, line);
        }

        // вывод: print E;
        if (code == LEX_PRINT) {
            Advance();
            auto expr = ParseExpression();
            if (CurrentCode() == LEX_SEMICOLON) Advance();
            return std::make_unique<PrintNode>(std::move(expr), line);
        }

        // возврат: return E;
        if (code == LEX_RETURN) {
            Advance();
            auto expr = ParseExpression();
            if (CurrentCode() == LEX_SEMICOLON) Advance();
            return std::make_unique<ReturnNode>(std::move(expr), line);
        }

        // цикл while (E) { ... };
        if (code == LEX_WHILE) {
            auto whileNode = std::make_unique<WhileNode>(line);
            Advance(); // 'w'
            if (CurrentCode() == LEX_LEFTHESIS) Advance(); // '('
            whileNode->condition = ParseExpression();
            if (CurrentCode() == LEX_RIGHTHESIS) Advance(); // ')'

            whileNode->body = ParseBlock();
            if (CurrentCode() == LEX_SEMICOLON) Advance(); // ';'
            return whileNode;
        }

        // идентификатор: либо присваивание (a = E;), либо вызов функции (foo(W);)
        if (code == LEX_ID) {
            int idIdx = Current().idxTI;
            Advance();

            if (CurrentCode() == LEX_EQUALS) {
                Advance(); // '='
                auto expr = ParseExpression();
                if (CurrentCode() == LEX_SEMICOLON) Advance();
                return std::make_unique<AssignNode>(idIdx, std::move(expr), line);
            }

            if (CurrentCode() == LEX_LEFTHESIS) {
                auto call = std::make_unique<CallNode>(idIdx, line);
                Advance(); // '('
                while (!AtEnd() && CurrentCode() != LEX_RIGHTHESIS) {
                    call->args.push_back(ParseExpression());
                    if (CurrentCode() == LEX_COMMA) {
                        Advance();
                    }
                }
                if (CurrentCode() == LEX_RIGHTHESIS) Advance(); // ')'
                if (CurrentCode() == LEX_SEMICOLON) Advance();  // ';'
                return std::make_unique<ExprStmtNode>(std::move(call), line);
            }
        }

        Advance();
        return nullptr;
    }

    // разбор выражений, начиная с самого низкого приоритета

    std::unique_ptr<ExprNode> Builder::ParseExpression() {
        return ParseComparison();
    }

    std::unique_ptr<ExprNode> Builder::ParseComparison() {
        auto left = ParseAddSubOr();

        while (!AtEnd() && CurrentCode() == LEX_COMP) {
            int line = Current().sn;
            std::string opStr = Current().op;
            Advance();
            auto right = ParseAddSubOr();
            left = std::make_unique<BinaryOpNode>(opStr, std::move(left), std::move(right), line);
        }

        return left;
    }

    std::unique_ptr<ExprNode> Builder::ParseAddSubOr() {
        auto left = ParseMulDivAnd();

        while (!AtEnd() && (CurrentCode() == LEX_PLUS || CurrentCode() == LEX_MINUS || CurrentCode() == LEX_BIT_OR)) {
            int line = Current().sn;
            std::string opStr = (CurrentCode() == LEX_PLUS) ? "+" :
                (CurrentCode() == LEX_MINUS) ? "-" : "||";
            Advance();
            auto right = ParseMulDivAnd();
            left = std::make_unique<BinaryOpNode>(opStr, std::move(left), std::move(right), line);
        }

        return left;
    }

    std::unique_ptr<ExprNode> Builder::ParseMulDivAnd() {
        auto left = ParseUnary();

        while (!AtEnd() && (CurrentCode() == LEX_STAR || CurrentCode() == LEX_DIRSLASH || CurrentCode() == LEX_BIT_AND)) {
            int line = Current().sn;
            std::string opStr = (CurrentCode() == LEX_STAR) ? "*" :
                (CurrentCode() == LEX_DIRSLASH) ? "/" : "&&";
            Advance();
            auto right = ParseUnary();
            left = std::make_unique<BinaryOpNode>(opStr, std::move(left), std::move(right), line);
        }

        return left;
    }

    std::unique_ptr<ExprNode> Builder::ParseUnary() {
        if (!AtEnd() && (CurrentCode() == LEX_MINUS || CurrentCode() == LEX_BIT_NOT)) {
            int line = Current().sn;
            char opChar = (CurrentCode() == LEX_MINUS) ? '-' : '~';
            Advance();
            auto operand = ParseUnary();
            return std::make_unique<UnaryOpNode>(opChar, std::move(operand), line);
        }
        return ParsePrimary();
    }

    std::unique_ptr<ExprNode> Builder::ParsePrimary() {
        int line = Current().sn;
        char code = CurrentCode();

        if (code == LEX_LEFTHESIS) {
            Advance(); // '('
            auto expr = ParseExpression();
            if (CurrentCode() == LEX_RIGHTHESIS) {
                Advance(); // ')'
            }
            return expr;
        }

        if (code == LEX_ID) {
            int idIdx = Current().idxTI;
            Advance();

            if (CurrentCode() == LEX_LEFTHESIS) {
                auto call = std::make_unique<CallNode>(idIdx, line);
                Advance(); // '('
                while (!AtEnd() && CurrentCode() != LEX_RIGHTHESIS) {
                    call->args.push_back(ParseExpression());
                    if (CurrentCode() == LEX_COMMA) {
                        Advance();
                    }
                }
                if (CurrentCode() == LEX_RIGHTHESIS) {
                    Advance(); // ')'
                }
                return call;
            }

            return std::make_unique<IdOrLitNode>(idIdx, line);
        }

        if (code == LEX_LITERAL) {
            int litIdx = Current().idxTI;
            Advance();
            return std::make_unique<IdOrLitNode>(litIdx, line);
        }

        Advance();
        return nullptr;
    }



    // отладочная печать готового дерева

    // Форматирует имя переменной, функции или значение литерала из таблицы IT
    std::string FormatITEntry(int idxTI, const IT::IdTable& it) {
        if (idxTI == MAIN_SCOPE) return "main";
        if (idxTI < 0 || idxTI >= it.size) return "<invalid>";

        const IT::Entry& entry = it.table[idxTI];
        if (entry.idtype == IT::L) {
            if (entry.iddatatype == IT::SHORT) {
                return std::to_string(entry.value.vshort) + " [lit:" + entry.id + "]";
            }
            if (entry.iddatatype == IT::CHAR) {
                return std::string("'") + entry.value.vchar + "' [lit:" + entry.id + "]";
            }
            if (entry.iddatatype == IT::STR) {
                std::string s(entry.value.vstr.str, entry.value.vstr.len);
                return "'" + s + "' [lit:" + entry.id + "]";
            }
        }
        return std::string(entry.id);
    }

    const char* DataTypeToStr(IT::IDDATATYPE type) {
        switch (type) {
        case IT::SHORT: return "short";
        case IT::STR:   return "string";
        case IT::CHAR:  return "char";
        default:        return "undef";
        }
    }

    // Рекурсивная печать узлов выражений
    void PrintExpr(const ExprNode* expr, const IT::IdTable& it, std::ostream& os,
        const std::string& prefix, bool isLast) {
        if (!expr) return;

        os << prefix << (isLast ? "+-- " : "|-- ");
        std::string childPrefix = prefix + (isLast ? "    " : "|   ");

        if (auto idLit = dynamic_cast<const IdOrLitNode*>(expr)) {
            os << "Id/Lit: " << FormatITEntry(idLit->idxTI, it) << "\n";
        }
        else if (auto unOp = dynamic_cast<const UnaryOpNode*>(expr)) {
            os << "UnaryOp (" << unOp->op << ")\n";
            PrintExpr(unOp->operand.get(), it, os, childPrefix, true);
        }
        else if (auto binOp = dynamic_cast<const BinaryOpNode*>(expr)) {
            os << "BinaryOp (" << binOp->op << ")\n";
            PrintExpr(binOp->left.get(), it, os, childPrefix, false);
            PrintExpr(binOp->right.get(), it, os, childPrefix, true);
        }
        else if (auto call = dynamic_cast<const CallNode*>(expr)) {
            os << "Call: " << FormatITEntry(call->funcIdxTI, it)
                << "() [args: " << call->args.size() << "]\n";
            for (size_t i = 0; i < call->args.size(); ++i) {
                PrintExpr(call->args[i].get(), it, os, childPrefix, i + 1 == call->args.size());
            }
        }
    }

    // Рекурсивная печать узлов операторов
    void PrintStmt(const StmtNode* stmt, const IT::IdTable& it, std::ostream& os,
        const std::string& prefix, bool isLast) {
        if (!stmt) return;

        os << prefix << (isLast ? "+-- " : "|-- ");
        std::string childPrefix = prefix + (isLast ? "    " : "|   ");

        if (auto decl = dynamic_cast<const VarDeclNode*>(stmt)) {
            IT::IDDATATYPE dt = (decl->varIdxTI >= 0 && decl->varIdxTI < it.size)
                ? it.table[decl->varIdxTI].iddatatype : IT::UNDEF;
            os << "VarDecl: " << DataTypeToStr(dt) << " "
                << FormatITEntry(decl->varIdxTI, it) << " (line " << decl->line << ")\n";
        }
        else if (auto assign = dynamic_cast<const AssignNode*>(stmt)) {
            os << "Assign: " << FormatITEntry(assign->varIdxTI, it)
                << " = (line " << assign->line << ")\n";
            PrintExpr(assign->expr.get(), it, os, childPrefix, true);
        }
        else if (auto inp = dynamic_cast<const InputNode*>(stmt)) {
            os << "Input: get " << FormatITEntry(inp->varIdxTI, it)
                << " (line " << inp->line << ")\n";
        }
        else if (auto prn = dynamic_cast<const PrintNode*>(stmt)) {
            os << "Print (line " << prn->line << ")\n";
            PrintExpr(prn->expr.get(), it, os, childPrefix, true);
        }
        else if (auto ret = dynamic_cast<const ReturnNode*>(stmt)) {
            os << "Return (line " << ret->line << ")\n";
            PrintExpr(ret->expr.get(), it, os, childPrefix, true);
        }
        else if (auto exprStmt = dynamic_cast<const ExprStmtNode*>(stmt)) {
            os << "ExprStmt (line " << exprStmt->line << ")\n";
            PrintExpr(exprStmt->expr.get(), it, os, childPrefix, true);
        }
        else if (auto wh = dynamic_cast<const WhileNode*>(stmt)) {
            os << "While (line " << wh->line << ")\n";
            os << childPrefix << "|-- Condition:\n";
            PrintExpr(wh->condition.get(), it, os, childPrefix + "|   ", true);

            os << childPrefix << "+-- Body [" << wh->body.size() << " stmts]:\n";
            for (size_t i = 0; i < wh->body.size(); ++i) {
                PrintStmt(wh->body[i].get(), it, os, childPrefix + "    ", i + 1 == wh->body.size());
            }
        }
    }


    void PrintAST(const ProgramNode* program, const IT::IdTable& it, std::ostream& os) {
        if (!program) return;

        os << "Program [functions: " << program->functions.size() << "]\n";

        for (size_t f = 0; f < program->functions.size(); ++f) {
            const auto& func = program->functions[f];
            bool isLastFunc = (f + 1 == program->functions.size());
            std::string funcPrefix = isLastFunc ? "    " : "|   ";

            os << (isLastFunc ? "+-- " : "|-- ");
            if (func->isMain) {
                os << "Main (line " << func->line << ")\n";
            }
            else {
                IT::IDDATATYPE retType = (func->funcIdxTI >= 0 && func->funcIdxTI < it.size)
                    ? it.table[func->funcIdxTI].iddatatype : IT::UNDEF;
                os << "Function: " << DataTypeToStr(retType) << " "
                    << FormatITEntry(func->funcIdxTI, it) << "(";
                for (size_t p = 0; p < func->paramIdxTI.size(); ++p) {
                    if (p > 0) os << ", ";
                    IT::IDDATATYPE pType = it.table[func->paramIdxTI[p]].iddatatype;
                    os << DataTypeToStr(pType) << " " << FormatITEntry(func->paramIdxTI[p], it);
                }
                os << ") (line " << func->line << ")\n";
            }

            for (size_t s = 0; s < func->body.size(); ++s) {
                PrintStmt(func->body[s].get(), it, os, funcPrefix, s + 1 == func->body.size());
            }
        }
    }
}