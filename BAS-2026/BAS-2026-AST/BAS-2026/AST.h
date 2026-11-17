#pragma once
#include <vector>
#include <string>
#include <memory>
#include "IT.h"

namespace AST {

    // Базовый узел дерева
    struct Node {
        int line = 0;
        virtual ~Node() = default;
    };

    // Базовый узел выражения (имеет вычисленный тип данных для семантики)
    struct ExprNode : Node {
        IT::IDDATATYPE evalType = IT::UNDEF;
    };

    // Литерал или переменная/параметр (ссылка на индекс в IT)
    struct IdOrLitNode : ExprNode {
        int idxTI;
        IdOrLitNode(int idx, int ln) : idxTI(idx) { line = ln; }
    };

    // Унарная операция: '-' или '~'
    struct UnaryOpNode : ExprNode {
        char op; // '-' или '~'
        std::unique_ptr<ExprNode> operand;
        UnaryOpNode(char o, std::unique_ptr<ExprNode> expr, int ln)
            : op(o), operand(std::move(expr)) {
            line = ln;
        }
    };

    // Бинарная операция: '+', '-', '*', '/', '&&', '||', '<', '>', '==', '!=', '<=', '>='
    struct BinaryOpNode : ExprNode {
        std::string op;
        std::unique_ptr<ExprNode> left;
        std::unique_ptr<ExprNode> right;
        BinaryOpNode(std::string o, std::unique_ptr<ExprNode> l, std::unique_ptr<ExprNode> r, int ln)
            : op(std::move(o)), left(std::move(l)), right(std::move(r)) {
            line = ln;
        }
    };

    // Вызов функции: pow(2, 5)
    struct CallNode : ExprNode {
        int funcIdxTI;
        std::vector<std::unique_ptr<ExprNode>> args;
        CallNode(int idx, int ln) : funcIdxTI(idx) { line = ln; }
    };

    // Базовый узел оператора
    struct StmtNode : Node {};

    // Выражение как отдельный оператор (например, вызов функции: test_numbers();)
    struct ExprStmtNode : StmtNode {
        std::unique_ptr<ExprNode> expr;
        ExprStmtNode(std::unique_ptr<ExprNode> e, int ln) : expr(std::move(e)) { line = ln; }
    };

    // Объявление переменной: short a;
    struct VarDeclNode : StmtNode {
        int varIdxTI;
        VarDeclNode(int idx, int ln) : varIdxTI(idx) { line = ln; }
    };

    // Присваивание: a = E;
    struct AssignNode : StmtNode {
        int varIdxTI;
        std::unique_ptr<ExprNode> expr;
        AssignNode(int idx, std::unique_ptr<ExprNode> e, int ln)
            : varIdxTI(idx), expr(std::move(e)) {
            line = ln;
        }
    };

    // Ввод и вывод: get a; / print E;
    struct InputNode : StmtNode {
        int varIdxTI;
        InputNode(int idx, int ln) : varIdxTI(idx) { line = ln; }
    };

    struct PrintNode : StmtNode {
        std::unique_ptr<ExprNode> expr;
        PrintNode(std::unique_ptr<ExprNode> e, int ln) : expr(std::move(e)) { line = ln; }
    };

    // Возврат из функции: return E;
    struct ReturnNode : StmtNode {
        std::unique_ptr<ExprNode> expr;
        ReturnNode(std::unique_ptr<ExprNode> e, int ln) : expr(std::move(e)) { line = ln; }
    };

    // Цикл while (cond) { body };
    struct WhileNode : StmtNode {
        std::unique_ptr<ExprNode> condition;
        std::vector<std::unique_ptr<StmtNode>> body;
        explicit WhileNode(int ln) { line = ln; }
    };

    // Объявление функции или main
    struct FunctionNode : Node {
        int funcIdxTI = MAIN_SCOPE;
        bool isMain = false;
        std::vector<int> paramIdxTI;
        std::vector<std::unique_ptr<StmtNode>> body;
    };

    // Корень программы
    struct ProgramNode : Node {
        std::vector<std::unique_ptr<FunctionNode>> functions;
    };
}
