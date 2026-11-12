#include "stdafx.h"
#include "CodeGen.h"
#include <sstream>

namespace CodeGen {

    Generator::Generator(const IT::IdTable& it, Out::OUT& out)
        : it_(it), out_(out), labelCounter_(0) {
    }

    std::string Generator::NewLabel(const std::string& prefix) {
        return prefix + "_" + std::to_string(++labelCounter_);
    }

    std::string Generator::GetMasmVarName(int idxTI) const {
        if (idxTI < 0 || idxTI >= it_.size) return "INVALID_VAR";
        return std::string("V_") + it_.table[idxTI].id + "_" + std::to_string(idxTI);
    }

    std::string Generator::GetMasmFuncName(int idxTI) const {
        if (idxTI == MAIN_SCOPE) return "main";
        if (idxTI < 0 || idxTI >= it_.size) return "INVALID_FUNC";

        const IT::Entry& entry = it_.table[idxTI];
        if (entry.idxfirstLE == -1) {
            return std::string("_std_") + entry.id;
        }
        return std::string("F_") + entry.id + "_" + std::to_string(idxTI);
    }

    void Generator::Generate(const AST::ProgramNode* program) {
        if (!program) return;

        EmitHeader();
        EmitDataSection(program);
        EmitCodeSection(program);
    }

    void Generator::EmitHeader() {
        std::ostringstream ss;
        ss << ".586\n"
            << ".model flat, stdcall\n"
            << "includelib libucrt.lib\n"
            << "includelib kernel32.lib\n"
            << "includelib StdLib.lib\n\n"
            << "ExitProcess PROTO :DWORD\n"
            << "SetConsoleCP PROTO :DWORD\n"
            << "SetConsoleOutputCP PROTO :DWORD\n\n"
            << "_std_print_short PROTO :SDWORD\n"
            << "_std_print_char  PROTO :DWORD\n"
            << "_std_print_str   PROTO :DWORD\n"
            << "_std_input_short PROTO\n"
            << "_std_input_char  PROTO\n"
            << "_std_pow         PROTO :SDWORD, :SDWORD\n"
            << "_std_itos        PROTO :SDWORD\n"
            << "_std_topolish    PROTO :DWORD\n\n";

        Out::WriteString(out_, ss.str());
    }

    void Generator::EmitDataSection(const AST::ProgramNode* program) {
        std::ostringstream ss;

        ss << ".const\n";
        ss << "    EMPTY_STR BYTE 0\n";

        for (int i = 0; i < it_.size; ++i) {
            const IT::Entry& entry = it_.table[i];
            if (entry.idtype != IT::L) continue;

            std::string masmName = GetMasmVarName(i);
            if (entry.iddatatype == IT::SHORT) {
                ss << "    " << masmName << " SDWORD " << entry.value.vshort << "\n";
            }
            else if (entry.iddatatype == IT::CHAR) {
                ss << "    " << masmName << " DWORD " << static_cast<int>(static_cast<unsigned char>(entry.value.vchar)) << "\n";
            }
            else if (entry.iddatatype == IT::STR) {
                ss << "    " << masmName << "_RAW BYTE ";
                if (entry.value.vstr.len == 0) {
                    ss << "0\n";
                }
                else {
                    for (int j = 0; j < entry.value.vstr.len; ++j) {
                        if (j > 0) ss << ", ";
                        ss << static_cast<int>(static_cast<unsigned char>(entry.value.vstr.str[j]));
                    }
                    ss << ", 0\n";
                }
                ss << "    " << masmName << " DWORD OFFSET " << masmName << "_RAW\n";
            }
        }

        ss << "\n.data\n";
        for (int i = 0; i < it_.size; ++i) {
            const IT::Entry& entry = it_.table[i];
            if (entry.idtype != IT::V && entry.idtype != IT::P) continue;

            std::string masmName = GetMasmVarName(i);
            if (entry.iddatatype == IT::SHORT) {
                ss << "    " << masmName << " SDWORD 0\n";
            }
            else if (entry.iddatatype == IT::CHAR) {
                ss << "    " << masmName << " DWORD 0\n";
            }
            else if (entry.iddatatype == IT::STR) {
                ss << "    " << masmName << " DWORD OFFSET EMPTY_STR\n";
            }
        }

        ss << "\n";
        Out::WriteString(out_, ss.str());
    }

    void Generator::EmitCodeSection(const AST::ProgramNode* program) {
        Out::WriteString(out_, ".code\n\n");

        for (const auto& func : program->functions) {
            if (!func->isMain) {
                EmitFunction(func.get());
            }
        }

        for (const auto& func : program->functions) {
            if (func->isMain) {
                EmitFunction(func.get());
            }
        }

        // Обработчик аварийного выхода при делении на 0 во время выполнения
        Out::WriteString(out_, "RUNTIME_DIV_ZERO:\n    push -1\n    call ExitProcess\n\nend main\n");
    }

    void Generator::EmitFunction(const AST::FunctionNode* func) {
        std::ostringstream ss;
        std::string funcName = func->isMain ? "main" : GetMasmFuncName(func->funcIdxTI);
        std::string exitLabel = NewLabel("EXIT_" + funcName);

        if (func->isMain) {
            ss << "main PROC\n";
            ss << "    push 1251\n"
                << "    call SetConsoleCP\n"
                << "    push 1251\n"
                << "    call SetConsoleOutputCP\n\n";
        }
        else {
            ss << funcName << " PROC";
            for (size_t i = 0; i < func->paramIdxTI.size(); ++i) {
                ss << (i == 0 ? " " : ", ") << "P_ARG_" << i << ":SDWORD";
            }
            ss << "\n";

            for (size_t i = 0; i < func->paramIdxTI.size(); ++i) {
                ss << "    mov eax, P_ARG_" << i << "\n";
                ss << "    mov " << GetMasmVarName(func->paramIdxTI[i]) << ", eax\n";
            }
            ss << "\n";
        }

        Out::WriteString(out_, ss.str());

        EmitBlock(func->body, exitLabel);

        std::ostringstream endSs;
        endSs << exitLabel << ":\n";
        if (func->isMain) {
            endSs << "    push 0\n"
                << "    call ExitProcess\n"
                << "main ENDP\n\n";
        }
        else {
            endSs << "    ret\n"
                << funcName << " ENDP\n\n";
        }

        Out::WriteString(out_, endSs.str());
    }

    void Generator::EmitBlock(const std::vector<std::unique_ptr<AST::StmtNode>>& stmts, const std::string& exitLabel) {
        for (const auto& stmt : stmts) {
            if (stmt) {
                EmitStatement(stmt.get(), exitLabel);
            }
        }
    }

    void Generator::EmitStatement(const AST::StmtNode* stmt, const std::string& exitLabel) {
        if (dynamic_cast<const AST::VarDeclNode*>(stmt)) {
            return;
        }

        if (auto assign = dynamic_cast<const AST::AssignNode*>(stmt)) {
            EmitExpr(assign->expr.get());
            std::ostringstream ss;
            if (it_.table[assign->varIdxTI].iddatatype == IT::SHORT) {
                ss << "    movsx eax, ax\n";
            }
            ss << "    mov " << GetMasmVarName(assign->varIdxTI) << ", eax\n";
            Out::WriteString(out_, ss.str());
            return;
        }

        if (auto inp = dynamic_cast<const AST::InputNode*>(stmt)) {
            std::ostringstream ss;
            IT::IDDATATYPE dt = it_.table[inp->varIdxTI].iddatatype;
            if (dt == IT::SHORT) {
                ss << "    call _std_input_short\n"
                    << "    movsx eax, ax\n"
                    << "    mov " << GetMasmVarName(inp->varIdxTI) << ", eax\n";
            }
            else if (dt == IT::CHAR) {
                ss << "    call _std_input_char\n"
                    << "    mov " << GetMasmVarName(inp->varIdxTI) << ", eax\n";
            }
            Out::WriteString(out_, ss.str());
            return;
        }

        if (auto prn = dynamic_cast<const AST::PrintNode*>(stmt)) {
            EmitExpr(prn->expr.get());
            std::ostringstream ss;
            ss << "    push eax\n";
            if (prn->expr->evalType == IT::SHORT) {
                ss << "    call _std_print_short\n";
            }
            else if (prn->expr->evalType == IT::CHAR) {
                ss << "    call _std_print_char\n";
            }
            else if (prn->expr->evalType == IT::STR) {
                ss << "    call _std_print_str\n";
            }
            Out::WriteString(out_, ss.str());
            return;
        }

        if (auto ret = dynamic_cast<const AST::ReturnNode*>(stmt)) {
            EmitExpr(ret->expr.get());
            std::ostringstream ss;
            if (ret->expr->evalType == IT::SHORT) {
                ss << "    movsx eax, ax\n";
            }
            ss << "    jmp " << exitLabel << "\n";
            Out::WriteString(out_, ss.str());
            return;
        }

        if (auto exprStmt = dynamic_cast<const AST::ExprStmtNode*>(stmt)) {
            EmitExpr(exprStmt->expr.get());
            return;
        }

        if (auto wh = dynamic_cast<const AST::WhileNode*>(stmt)) {
            std::string startLbl = NewLabel("WHILE_START");
            std::string endLbl = NewLabel("WHILE_END");

            Out::WriteString(out_, startLbl + ":\n");
            EmitExpr(wh->condition.get());

            std::ostringstream condSs;
            condSs << "    cmp ax, 0\n"
                << "    je " << endLbl << "\n";
            Out::WriteString(out_, condSs.str());

            EmitBlock(wh->body, exitLabel);

            std::ostringstream loopSs;
            loopSs << "    jmp " << startLbl << "\n"
                << endLbl << ":\n";
            Out::WriteString(out_, loopSs.str());
            return;
        }
    }

    void Generator::EmitExpr(const AST::ExprNode* expr) {
        if (!expr) return;

        if (auto idLit = dynamic_cast<const AST::IdOrLitNode*>(expr)) {
            std::ostringstream ss;
            ss << "    mov eax, " << GetMasmVarName(idLit->idxTI) << "\n";
            Out::WriteString(out_, ss.str());
            return;
        }

        if (auto unOp = dynamic_cast<const AST::UnaryOpNode*>(expr)) {
            EmitExpr(unOp->operand.get());
            std::ostringstream ss;
            if (unOp->op == '-') {
                ss << "    neg ax\n"
                    << "    movsx eax, ax\n";
            }
            else if (unOp->op == '~') {
                ss << "    not ax\n"
                    << "    movsx eax, ax\n";
            }
            Out::WriteString(out_, ss.str());
            return;
        }
        if (auto binOp = dynamic_cast<const AST::BinaryOpNode*>(expr)) {
            EmitExpr(binOp->left.get());
            Out::WriteString(out_, "    push eax\n");
            EmitExpr(binOp->right.get());
            Out::WriteString(out_, "    mov ebx, eax\n    pop eax\n");
            std::ostringstream ss;
            if (binOp->op == "+") {
                ss << "    add ax, bx\n"
                    << "    movsx eax, ax\n";
            }
            else if (binOp->op == "-") {
                ss << "    sub ax, bx\n"
                    << "    movsx eax, ax\n";
            }
            else if (binOp->op == "*") {
                ss << "    imul bx\n"
                    << "    movsx eax, ax\n";
            }
            else if (binOp->op == "/") {
                ss << "    cmp bx, 0\n"
                    << "    je RUNTIME_DIV_ZERO\n"
                    << "    cwd\n"
                    << "    idiv bx\n"
                    << "    movsx eax, ax\n";
            }
            else if (binOp->op == "&&") {
                ss << "    and ax, bx\n"
                    << "    movsx eax, ax\n";
            }
            else if (binOp->op == "||") {
                ss << "    or ax, bx\n"
                    << "    movsx eax, ax\n";
            }
            else {
                std::string trueLbl = NewLabel("CMP_TRUE");
                std::string doneLbl = NewLabel("CMP_DONE");
                std::string jmpCmd = "je";
                if (binOp->op == "==") jmpCmd = "je";
                else if (binOp->op == "!=") jmpCmd = "jne";
                else if (binOp->op == "<")  jmpCmd = "jl";
                else if (binOp->op == ">")  jmpCmd = "jg";
                else if (binOp->op == "<=") jmpCmd = "jle";
                else if (binOp->op == ">=") jmpCmd = "jge";
                ss << "    cmp ax, bx\n"
                    << "    " << jmpCmd << " " << trueLbl << "\n"
                    << "    mov eax, 0\n"
                    << "    jmp " << doneLbl << "\n"
                    << trueLbl << ":\n"
                    << "    mov eax, 1\n"
                    << doneLbl << ":\n";
            }
            Out::WriteString(out_, ss.str());
            return;
        }        
        if (auto call = dynamic_cast<const AST::CallNode*>(expr)) {
            for (int i = (int)call->args.size() - 1; i >= 0; --i) {
                EmitExpr(call->args[i].get());
                Out::WriteString(out_, "    push eax\n");
            }
            std::ostringstream ss;
            ss << "    call " << GetMasmFuncName(call->funcIdxTI) << "\n";
            Out::WriteString(out_, ss.str());
            return;
        }
    }
    void Run(const AST::ProgramNode* program, const IT::IdTable& it, Out::OUT& out) {
        Generator gen(it, out);
        gen.Generate(program);
    }
} // namespace CodeGen