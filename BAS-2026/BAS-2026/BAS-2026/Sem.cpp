#include "stdafx.h"
#include "Sem.h"
#include "Error.h"

namespace Semantic {

    Analyzer::Analyzer(IT::IdTable& it) : it_(it) {}

    bool Analyzer::Analyze(AST::ProgramNode* program) {
        if (!program) return false;

        // 1. Проверка точки входа main (ошибки 701 и 702)
        int mainCount = 0;
        for (const auto& func : program->functions) {
            if (func->isMain) {
                mainCount++;
                if (mainCount > 1) {
                    throw ERROR_THROW_LINE(702, func->line); // Задано более одной точки входа main
                }
            }
        }
        if (mainCount == 0) {
            throw ERROR_THROW(701); // Отсутствует точка входа main
        }

        // 2. Анализируем каждую функцию и main
        for (const auto& func : program->functions) {
            AnalyzeFunction(func.get());
        }

        return true;
    }

    void Analyzer::AnalyzeFunction(AST::FunctionNode* func) {
        isCurrentMain_ = func->isMain;
        hasReturnStmt_ = false;
        whileDepth_ = 0;

        if (isCurrentMain_) {
            currentReturnType_ = IT::SHORT; // main возвращает код выхода (return 0;)
        }
        else {
            if (func->funcIdxTI < 0 || func->funcIdxTI >= it_.size) {
                throw ERROR_THROW_LINE(208, func->line);
            }
            const IT::Entry& funcEntry = it_.table[func->funcIdxTI];
            currentReturnType_ = funcEntry.iddatatype;

            // Проверка лимита параметров функции (ошибка 706)
            if (func->paramIdxTI.size() > IT_MAX_PARAMS) {
                throw ERROR_THROW_LINE(706, func->line);
            }
        }

        AnalyzeBlock(func->body);

        // Проверяем, что в функции есть хотя бы один оператор return (ошибка 723)
        if (!isCurrentMain_ && !hasReturnStmt_) {
            throw ERROR_THROW_LINE(723, func->line);
        }
    }

    void Analyzer::AnalyzeBlock(const std::vector<std::unique_ptr<AST::StmtNode>>& stmts) {
        for (const auto& stmt : stmts) {
            if (stmt) {
                AnalyzeStatement(stmt.get());
            }
        }
    }

    void Analyzer::AnalyzeStatement(AST::StmtNode* stmt) {
        // 1. Объявление переменной (short a;)
        if (dynamic_cast<AST::VarDeclNode*>(stmt)) {
            return;
        }

        // 2. Присваивание: LHS = RHS;
        if (auto assign = dynamic_cast<AST::AssignNode*>(stmt)) {
            const IT::Entry& lhsEntry = it_.table[assign->varIdxTI];
            IT::IDDATATYPE rhsType = AnalyzeExpr(assign->expr.get());

            if (lhsEntry.iddatatype != rhsType) {
                throw ERROR_THROW_LINE(710, assign->line); // Несовпадение типов при присваивании
            }
            return;
        }

        // 3. Ввод: get a;
        if (auto inp = dynamic_cast<AST::InputNode*>(stmt)) {
            const IT::Entry& target = it_.table[inp->varIdxTI];
            if (target.idtype != IT::V && target.idtype != IT::P) {
                throw ERROR_THROW_LINE(710, inp->line);
            }
            return;
        }

        // 4. Вывод: print E;
        if (auto prn = dynamic_cast<AST::PrintNode*>(stmt)) {
            AnalyzeExpr(prn->expr.get());
            return;
        }

        // 5. Возврат из функции: return E;
        if (auto ret = dynamic_cast<AST::ReturnNode*>(stmt)) {
            hasReturnStmt_ = true;
            IT::IDDATATYPE retExprType = AnalyzeExpr(ret->expr.get());

            if (retExprType != currentReturnType_) {
                throw ERROR_THROW_LINE(715, ret->line); // Тип функции и тип return должны совпадать
            }
            return;
        }

        // 6. Вызов функции как отдельный оператор: foo(a, b);
        if (auto exprStmt = dynamic_cast<AST::ExprStmtNode*>(stmt)) {
            AnalyzeExpr(exprStmt->expr.get());
            return;
        }

        // 7. Цикл while (condition) { body };
        if (auto wh = dynamic_cast<AST::WhileNode*>(stmt)) {
            IT::IDDATATYPE condType = AnalyzeExpr(wh->condition.get());
            if (condType != IT::SHORT) {
                throw ERROR_THROW_LINE(707, wh->line); // Условие while должно быть типа short
            }

            whileDepth_++;
            AnalyzeBlock(wh->body);
            whileDepth_--;
            return;
        }
    }

    IT::IDDATATYPE Analyzer::AnalyzeExpr(AST::ExprNode* expr) {
        if (!expr) return IT::UNDEF;

        // А. Идентификатор или литерал
        if (auto idLit = dynamic_cast<AST::IdOrLitNode*>(expr)) {
            const IT::Entry& entry = it_.table[idLit->idxTI];

            // Запрещаем использовать имя функции как переменную без вызова ()
            if (entry.idtype == IT::F) {
                throw ERROR_THROW_LINE(711, idLit->line);
            }

            expr->evalType = entry.iddatatype;
            return expr->evalType;
        }

        // Б. Унарные операции ('-' и '~')
        if (auto unOp = dynamic_cast<AST::UnaryOpNode*>(expr)) {
            IT::IDDATATYPE operandType = AnalyzeExpr(unOp->operand.get());

            if (operandType != IT::SHORT) {
                throw ERROR_THROW_LINE(712, unOp->line);
            }

            expr->evalType = IT::SHORT;
            return IT::SHORT;
        }

        // В. Бинарные операции (+, -, *, /, &&, ||, <, >, ==, !=, <=, >=)
        if (auto binOp = dynamic_cast<AST::BinaryOpNode*>(expr)) {
            IT::IDDATATYPE leftType = AnalyzeExpr(binOp->left.get());
            IT::IDDATATYPE rightType = AnalyzeExpr(binOp->right.get());

            // 1. Операции сравнения (<, >, ==, !=, <=, >=)
            if (binOp->op == "<" || binOp->op == ">" || binOp->op == "==" ||
                binOp->op == "!=" || binOp->op == "<=" || binOp->op == ">=") {
                if (leftType != rightType) {
                    throw ERROR_THROW_LINE(711, binOp->line);
                }
                if (leftType == IT::STR) {
                    throw ERROR_THROW_LINE(713, binOp->line);
                }
                expr->evalType = IT::SHORT;
                return IT::SHORT;
            }

            // 2. Побитовые операции (&& и ||)
            if (binOp->op == "&&" || binOp->op == "||") {
                if (leftType != IT::SHORT || rightType != IT::SHORT) {
                    throw ERROR_THROW_LINE(713, binOp->line);
                }
                expr->evalType = IT::SHORT;
                return IT::SHORT;
            }

            // 3. Арифметика (+, -, *, /)
            if (leftType != IT::SHORT || rightType != IT::SHORT) {
                throw ERROR_THROW_LINE(712, binOp->line);
            }

            // Проверка деления на ноль (ошибка 714)
            if (binOp->op == "/" && IsZeroConstant(binOp->right.get())) {
                throw ERROR_THROW_LINE(714, binOp->line);
            }

            expr->evalType = IT::SHORT;
            return IT::SHORT;
        }

        // Г. Вызов функции: func(arg1, arg2, ...)
        if (auto call = dynamic_cast<AST::CallNode*>(expr)) {
            const IT::Entry& funcEntry = it_.table[call->funcIdxTI];

            if (funcEntry.idtype != IT::F) {
                throw ERROR_THROW_LINE(711, call->line);
            }

            int expectedCount = funcEntry.func_meta.n_params;
            int actualCount = static_cast<int>(call->args.size());

            // Слишком много аргументов (ошибка 717)
            if (actualCount > expectedCount) {
                throw ERROR_THROW_LINE(717, call->line);
            }
            // Слишком мало аргументов (ошибка 718)
            if (actualCount < expectedCount) {
                throw ERROR_THROW_LINE(718, call->line);
            }

            // Проверка типов каждого переданного аргумента (ошибка 716)
            for (int i = 0; i < actualCount; ++i) {
                IT::IDDATATYPE argType = AnalyzeExpr(call->args[i].get());
                IT::IDDATATYPE expectedType = funcEntry.func_meta.params_types[i];

                if (argType != expectedType) {
                    throw ERROR_THROW_LINE(716, call->line);
                }
            }

            expr->evalType = funcEntry.iddatatype;
            return expr->evalType;
        }

        return IT::UNDEF;
    }

    bool Analyzer::IsZeroConstant(const AST::ExprNode* expr) const {
        if (auto idLit = dynamic_cast<const AST::IdOrLitNode*>(expr)) {
            const IT::Entry& entry = it_.table[idLit->idxTI];
            if (entry.idtype == IT::L && entry.iddatatype == IT::SHORT && entry.value.vshort == 0) {
                return true;
            }
        }
        if (auto unOp = dynamic_cast<const AST::UnaryOpNode*>(expr)) {
            if (unOp->op == '-') {
                return IsZeroConstant(unOp->operand.get());
            }
        }
        return false;
    }

    bool Run(AST::ProgramNode* program, IT::IdTable& it) {
        Analyzer analyzer(it);
        return analyzer.Analyze(program);
    }

} // namespace Semantic
