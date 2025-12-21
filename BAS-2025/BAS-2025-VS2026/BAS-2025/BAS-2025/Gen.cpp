#include "Gen.h"
#include "PN.h"

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


    IT::IDDATATYPE getExpDataType(LT::LexTable lextable, IT::IdTable idtable, int i, int end) {
        IT::Entry info;
        LT::Entry entry;
        char lexema;
        for (; i < end; i++) {
			entry = lextable.table[i];
            lexema = entry.lexema[0];
            if (lexema == LEX_ID || lexema == LEX_LITERAL) {
			    info = idtable.table[entry.idxTI];
                return info.iddatatype;
            }
        }
        return IT::UNDEF;
    }



    void Generate(LT::LexTable lextable, IT::IdTable idtable, Out::OUT out) {
        IT::IDDATATYPE datatype;
        IT::Entry info;
        LT::Entry entry;
        char lexema;
        int start;
        int end;
        string valueStr;


        Out::WriteString(out, "function random(min, max) {");
        Out::WriteString(out, "min = Math.ceil(min);");
        Out::WriteString(out, "max = Math.floor(max);");
        Out::WriteString(out, "return Math.floor(Math.random() * (max - min + 1)) + min;}");


        for (int i = 0; i < lextable.size; i++) {
			valueStr = "";
			entry = lextable.table[i];
            lexema = entry.lexema[0];
            start = 0;
            end = 0;

            cout << lexema;

            switch (lexema)
            {
            case LEX_FUNCTION:

                i += 1;
				entry = lextable.table[i];
				info = idtable.table[entry.idxTI];
                i += 1;
                Out::WriteString(out, "function ");
                Out::WriteString(out, info.id);

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
                        break;

                    case LEX_COMMA:
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
                break;



            case LEX_ID:
            case LEX_LITERAL:
                if (lextable.table[i - 1].lexema[0] == LEX_STRING) {
					info = idtable.table[entry.idxTI];
					Out::WriteString(out, "let ");
					Out::WriteString(out, info.id);

                    if(info.iddatatype == IT::UINT) Out::WriteString(out, " = 0");
                    if(info.iddatatype == IT::CHAR) Out::WriteString(out, " = '$'");
                    if(info.iddatatype == IT::STR) Out::WriteString(out, " = 'S'");

					Out::WriteString(out, ";\n");
                    i += 1; 
                }
                else {
                    start = i;
                    end = i;
                    for (; i < lextable.size; i++) {
						entry = lextable.table[i];
						lexema = entry.lexema[0];

                        if (lexema == LEX_SEMICOLON ||
                            lexema == FILLER_CHAR ||
                            lexema == LEX_PRINT ||
                            lexema == LEX_RETURN) {
                            end = i;
                            break;
                        }
                    }

                    if (lextable.table[end - 1].lexema[0] == LEX_EQUALS) {
						entry = lextable.table[start];
						info = idtable.table[entry.idxTI];

                        valueStr = PolishToJSExpression(lextable, idtable, start + 1, end - 1);
						if (getExpDataType(lextable, idtable, start, start + 1) == IT::UINT) 
							valueStr = "(" + valueStr + ") >>> 0";
                        valueStr = string(info.id) + " = " + valueStr + ";\n";
                    }
                    else {
						valueStr = PolishToJSExpression(lextable, idtable, start, end);
						if (getExpDataType(lextable, idtable, start, end) == IT::UINT) 
							valueStr = "(" + valueStr + ") >>> 0";
						if (lexema == LEX_PRINT) valueStr = "console.log(" + valueStr + ")";
						else if (lexema == LEX_RETURN) valueStr = "return " + valueStr ;
						valueStr = valueStr + ";\n";
                    }
					Out::WriteString(out, valueStr);
                }
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

				if (info.iddatatype == IT::UINT) {
					valueStr = std::to_string(info.value.vint);
				}
				else if (info.iddatatype == IT::CHAR) {
					valueStr = "'";
					valueStr += info.value.vchar;
					valueStr += "'";
				}
				else {
					valueStr = "'";
					for (int j = 0; j < info.value.vstr[0].len; j++) {
						valueStr += info.value.vstr[0].str[j];
					}
					valueStr += "'";
				}

				Out::WriteString(out, "case ");
				Out::WriteString(out, valueStr);
				Out::WriteString(out, ":\n");
                break;


            case LEX_SWITCH:
				Out::WriteString(out, "switch (");
                i += 1;
                i += 1;

                start = (int)i;
                end = (int)i; 

				for (; i < lextable.size; i += 1) {
					entry = lextable.table[i];
					lexema = entry.lexema[0];

					if (lexema == LEX_RIGHTHESIS) {
						end = i;
						break;
					}
				}
                i += 1; 

				valueStr = PolishToJSExpression(lextable, idtable, start, end);
				if (getExpDataType(lextable, idtable, start, end) == IT::UINT) 
					valueStr = "(" + valueStr + ") >>> 0";
				if (lexema == LEX_PRINT) valueStr = "console.log(" + valueStr + ")";
				else if (lexema == LEX_RETURN) valueStr = "return " + valueStr ;
				Out::WriteString(out, valueStr);
				Out::WriteString(out, ") {\n");

                break;


            case LEX_MAIN:
				Out::WriteString(out, "function main() {\n");
                i += 1;
                i += 1;
                break;
            }
        }

		Out::WriteString(out, "main();");
    }
}