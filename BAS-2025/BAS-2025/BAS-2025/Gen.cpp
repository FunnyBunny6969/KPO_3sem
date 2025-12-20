#include "Gen.h"

#include <fstream>
#include <string>
#include <vector>
#include <stack>

#include <iostream>
using namespace std;

namespace JS_CodeGeneration {


    std::string PolishToJSExpression(
        LT::LexTable& lextable, 
        IT::IdTable& idtable, 
        int start, 
        int end) 
    {
        std::stack<std::string> exprStack;

        auto getJSOperator = [](char op) -> std::string {
            switch (op) {
            case LEX_EQUALS: return "=";
            case LEX_PLUS: return "+";
            case LEX_MINUS: return "-";
            case LEX_STAR: return "*";
            case LEX_DIRSLASH: return "/";
            case LEX_BIT_AND: return "&&";
            case LEX_BIT_OR: return "||";
            case LEX_BIT_NOT: return "!";
            default: return std::string(1, op);
            }
        };

        int currentPos;
        for (int i = start; i < end; i++) {
            currentPos = i;
            LT::Entry& lex = lextable.table[i];
            if (lex.lexema[0] == '#') continue;


            if (lex.lexema[0] == LEX_LITERAL) {
                IT::Entry& entry = idtable.table[lex.idxTI];
                if (entry.idtype == IT::L) {
                    if (entry.iddatatype == IT::STR) {
                        exprStack.push("\"" + std::string(entry.value.vstr->str) + "\"");
                    }
                    else if (entry.iddatatype == IT::UINT || entry.iddatatype == IT::UINT) {
                        exprStack.push(std::to_string(entry.value.vint));
                    }
                    else if (entry.iddatatype == IT::CHAR) {
                        exprStack.push("'" + std::string(1, (char)entry.value.vint) + "'");
                    }
                    else {
                        exprStack.push(std::to_string(entry.value.vint));
                    }
                }
            }
            

            else if (lex.lexema[0] == LEX_ID) {
                IT::Entry& entry = idtable.table[lex.idxTI];

                if (entry.idtype == IT::F) {
                    int paramCount = entry.func_meta.n_params;

                    std::vector<std::string> args;
                    for (int j = 0; j < paramCount; j++) {
                        args.insert(args.begin(), exprStack.top());
                        exprStack.pop();
                    }

                    std::string call;
					call = std::string(entry.id) + "(";
					for (size_t j = 0; j < args.size(); j++) {
						if (j > 0) call += ", ";
						call += args[j];
					}
					call += ")";
                    exprStack.push(call);
                }
                else {
                    exprStack.push(entry.id);
                }
            }

            else {
                char op = lex.lexema[0];
                std::string jsOp = getJSOperator(op);


                bool isUnary = (op == LEX_BIT_NOT);
                bool isBinary = (
                    op == LEX_PLUS || 
                    op == LEX_MINUS || 
                    op == LEX_STAR || 
                    op == LEX_DIRSLASH ||
                    op == LEX_EQUALS || 
                    op == LEX_BIT_AND || 
                    op == LEX_BIT_OR);

                if (isUnary) {
                    std::string operand = exprStack.top();
                    exprStack.pop();

                    if (op == LEX_BIT_NOT) {
                        exprStack.push("!(" + operand + ")");
                    }
                    else {
                        exprStack.push(jsOp + operand);
                    }
                }
                else if (isBinary) {
                    std::string right = exprStack.top();
                    exprStack.pop();
                    std::string left = exprStack.top();
                    exprStack.pop();

                    if (op == LEX_EQUALS) {
                        exprStack.push(left + " = " + right);
                    }
                    else if (op == LEX_BIT_AND || op == LEX_BIT_OR) {
                        exprStack.push("(" + left + " " + jsOp + " " + right + ")");
                    }
                    else {
                        exprStack.push(left + " " + jsOp + " " + right);
                    }
                }
                else {
                    exprStack.push(std::string(1, op));
                }
            }
        }

        if (exprStack.empty()) {
            return "";
        }
        return exprStack.top();
    }










    void Generate(LT::LexTable lextable, IT::IdTable idtable, Out::OUT out) {

        IT::IDDATATYPE datatype;
        IT::Entry info;
        LT::Entry entry;
        char lexema;






        for (int i = 0; i < lextable.size; i++) {
			entry = lextable.table[i];
            lexema = entry.lexema[0];
            int start = 0;



            switch (lexema)
            {
            case LEX_FUNCTION:
                Out::WriteString(out, "function ");

                for (; i < lextable.size; i++) {
                    bool stop = false; 

					entry = lextable.table[i];
					lexema = entry.lexema[0];

                    switch (lexema)
                    {
                    case LEX_LEFTHESIS:
						Out::WriteString(out, "( ");
                        break;

                    case LEX_ID:
						info = idtable.table[entry.idxTI];
						Out::WriteString(out, info.id);
						Out::WriteString(out, ", ");
                        break;

                    case LEX_RIGHTHESIS:
						Out::WriteString(out, ") ");
                        break;

                    case LEX_LEFTBRACE:
						Out::WriteString(out, "{\n");
                        stop = true;
                        break;
                    }

                    if (stop) break;
                }
                i++;

				entry = lextable.table[i];
				lexema = entry.lexema[0];
                cout << "FUNC OPRED END" << lexema;
                break;



            case LEX_ID:
                if (lextable.table[i - 1].lexema[0] == LEX_STRING) {
					info = idtable.table[entry.idxTI];
					Out::WriteString(out, "let ");
					Out::WriteString(out, info.id);
					Out::WriteString(out, ";\n");
                    i++; i++;
                }
                else {
                    int start = i; int end;
                    for (; i < lextable.size; i++) {
						entry = lextable.table[i];
						lexema = entry.lexema[0];

                        if (lexema == LEX_SEMICOLON) {
                            end = i;
                            break;
                        }
                    }
                    i++;

                    
                    cout << "EXP: ";
                    for (; start < end; start++) {
						entry = lextable.table[start];
						lexema = entry.lexema[0];
						cout << lexema;
                    }
                }

				entry = lextable.table[i];
				lexema = entry.lexema[0];
                cout << "ID END" << lexema;
                break;


            case LEX_BREAK:
				Out::WriteString(out, "break;\n");
                break;


            case LEX_DEFAULT:
				Out::WriteString(out, "default: \n");
                break;


            case LEX_RIGHTBRACE:
				Out::WriteString(out, "};\n");
                break;


            case LEX_CASE:
                i++;
				entry = lextable.table[i];
				info = idtable.table[entry.idxTI];
				Out::WriteString(out, "case ");
				Out::WriteString(out, info.id);
				Out::WriteString(out, ":\n");
                break;


            case LEX_SWITCH:
				Out::WriteString(out, "switch (");
                i++;i++;

				start = i; int end;
				for (; i < lextable.size; i++) {
					entry = lextable.table[i];
					lexema = entry.lexema[0];

					if (lexema == LEX_RIGHTHESIS) {
						end = i;
						break;
					}
				}
                i++; i++;

				cout << "EXP: ";
                    for (; start < end; start++) {
						entry = lextable.table[start];
						lexema = entry.lexema[0];
						cout << lexema;
                    }

				Out::WriteString(out, ") {\n");

				entry = lextable.table[i];
				lexema = entry.lexema[0];
                cout << "SWITCH END" << lexema;
                break;


            case LEX_MAIN:
				Out::WriteString(out, "function main() {\n");
                i++;i++;
                break;
            }
        }

		Out::WriteString(out, "main();");
    }
}