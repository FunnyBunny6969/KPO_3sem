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
            case LEX_BIT_AND: return "&";
            case LEX_BIT_OR: return "|";
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
        Out::WriteString(out, "return Math.floor(Math.random() * (max - min + 1)) + min;}\n");

        Out::WriteString(out, "function pow(a, b) {");
        Out::WriteString(out, "return Math.pow(a, b);}\n\n");


        for (int i = 0; i < lextable.size; i++) {
            if (i >= lextable.size) break;


			valueStr = "";
			entry = lextable.table[i];
            lexema = entry.lexema[0];
            start = 0;
            end = 0;


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
				info = idtable.table[entry.idxTI];
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
                break;
            }
        }

		Out::WriteString(out, "main();");
    }
}






namespace ASM_CodeGeneration {


    std::string PolishToASMExpression(
        LT::LexTable& lextable,
        IT::IdTable& idtable,
        int start,
        int end)
    {
        std::stack<std::string> exprStack;
        std::string asmCode;

        auto getAsmOperator = [](char op) -> std::string {
            switch (op) {
            case LEX_PLUS: return "add";
            case LEX_MINUS: return "sub";
            case LEX_STAR: return "imul";
            case LEX_DIRSLASH: return "idiv";
            case LEX_BIT_AND: return "and";
            case LEX_BIT_OR: return "or";
            case LEX_BIT_NOT: return "not";
            case LEX_EQUALS: return "mov";
            default: return "";
            }
            };

        for (int i = start; i < end; i++) {
            LT::Entry& lex = lextable.table[i];
            if (lex.lexema[0] == '#') continue;

            if (lex.lexema[0] == LEX_LITERAL) {
                IT::Entry& entry = idtable.table[lex.idxTI];
                if (entry.idtype == IT::L) {
                    std::string operand;

                    if (entry.iddatatype == IT::STR) {
                        operand = "offset " + std::string(entry.id);  // Адрес строки
                    }
                    else if (entry.iddatatype == IT::UINT) {
                        operand = std::to_string(entry.value.vint);
                    }
                    else if (entry.iddatatype == IT::CHAR) {
                        operand = std::to_string(static_cast<int>(entry.value.vchar));
                    }

                    exprStack.push(operand);
                    asmCode += "    push " + operand + "\n";
                }
            }
            else if (lex.lexema[0] == LEX_ID) {
                IT::Entry& entry = idtable.table[lex.idxTI];

                if (entry.idtype == IT::F) {
                    int paramCount = entry.func_meta.n_params;
                    std::vector<std::string> args;

                    // Забираем аргументы из стека
                    for (int j = 0; j < paramCount; j++) {
                        if (!exprStack.empty()) {
                            args.insert(args.begin(), exprStack.top());
                            exprStack.pop();
                        }
                    }

                    // Генерируем call функции
                    std::string callCode = "    call " + std::string(entry.id) + "\n";
                    asmCode += callCode;
                    asmCode += "    push eax\n";  // Результат функции в стек
                    exprStack.push("eax");  // Помещаем результат в стек выражений
                }
                else {
                    // Переменная или параметр
                    std::string varName = entry.id;
                    exprStack.push(varName);
                    asmCode += "    push dword ptr [" + varName + "]\n";
                }
            }
            else {
                char op = lex.lexema[0];
                std::string asmOp = getAsmOperator(op);

                bool isUnary = (op == LEX_BIT_NOT);
                bool isBinary = (op == LEX_PLUS || op == LEX_MINUS ||
                    op == LEX_STAR || op == LEX_DIRSLASH ||
                    op == LEX_EQUALS || op == LEX_BIT_AND ||
                    op == LEX_BIT_OR);

                if (isUnary) {
                    if (!exprStack.empty()) {
                        std::string operand = exprStack.top();
                        exprStack.pop();

                        asmCode += "    pop eax\n";
                        asmCode += "    " + asmOp + " eax\n";
                        asmCode += "    push eax\n";

                        exprStack.push("eax");
                    }
                }
                else if (isBinary) {
                    if (exprStack.size() >= 2) {
                        std::string right = exprStack.top();
                        exprStack.pop();
                        std::string left = exprStack.top();
                        exprStack.pop();

                        if (op == LEX_EQUALS) {
                            // Присваивание: left = right
                            asmCode += "    pop eax\n";        // Правая часть
                            asmCode += "    pop ebx\n";        // Левая часть (адрес)
                            asmCode += "    mov [ebx], eax\n"; // Присваивание
                            asmCode += "    push eax\n";       // Результат
                        }
                        else if (op == LEX_DIRSLASH) {
                            // Деление
                            asmCode += "    pop ebx\n";    // Делитель
                            asmCode += "    pop eax\n";    // Делимое
                            asmCode += "    cdq\n";        // Расширяем eax до edx:eax
                            asmCode += "    idiv ebx\n";   // Деление
                            asmCode += "    push eax\n";   // Результат (частное)
                        }
                        else {
                            // Другие бинарные операции
                            asmCode += "    pop ebx\n";
                            asmCode += "    pop eax\n";
                            asmCode += "    " + asmOp + " eax, ebx\n";
                            asmCode += "    push eax\n";
                        }

                        exprStack.push("eax");
                    }
                }
            }
        }

        // Если в стеке что-то осталось - это результат
        if (!exprStack.empty()) {
            asmCode += "    pop eax\n";  // Результат в eax
        }

        return asmCode;
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


    void GenerateHeader(IT::IdTable idtable, Out::OUT out) {
        Out::WriteString(out, ".586\n");
        Out::WriteString(out, ".model flat, stdcall\n");
        Out::WriteString(out, "option casemap:none\n\n");

        Out::WriteString(out, "include windows.inc\n");
        Out::WriteString(out, "includelib kernel32.lib\n");
        Out::WriteString(out, "includelib user32.lib\n\n");

        Out::WriteString(out, "GetStdHandle proto :dword\n");
        Out::WriteString(out, "WriteConsoleA proto :dword, :dword, :dword, :dword, :dword\n");
        Out::WriteString(out, "ExitProcess proto :dword\n");
        Out::WriteString(out, "GetTickCount proto\n\n");

        // Секция данных
        Out::WriteString(out, ".data\n");

        for (int i = 0; i < idtable.size; i++) {
            IT::Entry entry = idtable.table[i];
            if (entry.idtype == IT::L) {
                if (entry.iddatatype == IT::STR) {
                    Out::WriteString(out, entry.id);
                    Out::WriteString(out, " BYTE '");
                    Out::WriteString(out, entry.value.vstr->str);
                    Out::WriteString(out, "', 0\n");
                } 
                else if (entry.iddatatype == IT::UINT) {
                    Out::WriteString(out, entry.id);
                    Out::WriteString(out, " DWORD ");
                    Out::WriteString(out, to_string(entry.value.vint));
                    Out::WriteString(out, "\n");
                }
                else if (entry.iddatatype == IT::CHAR) {
                    Out::WriteString(out, entry.id);
                    Out::WriteString(out, " BYTE '");
                    Out::WriteString(out, to_string(entry.value.vchar));
                    Out::WriteString(out, "', 0\n");
                } 
            }
            else if (entry.idtype == IT::V || entry.idtype == IT::P) {
                if (entry.iddatatype == IT::STR) {
                    Out::WriteString(out, entry.id);
                    Out::WriteString(out, "_");
                    if(entry.scope == MAIN_SCOPE) Out::WriteString(out, "main");
                    else if(entry.scope == GLOBAL_SCOPE) Out::WriteString(out, "glob");
                    else Out::WriteString(out, idtable.table[entry.scope].id);
                    Out::WriteString(out, " BYTE 'S', 0\n");
                } 
                else if (entry.iddatatype == IT::UINT) {
                    Out::WriteString(out, entry.id);
                    Out::WriteString(out, "_");
                    if(entry.scope == MAIN_SCOPE) Out::WriteString(out, "main");
                    else if(entry.scope == GLOBAL_SCOPE) Out::WriteString(out, "glob");
                    else Out::WriteString(out, idtable.table[entry.scope].id);
                    Out::WriteString(out, " DWORD 0\n");
                }
                else if (entry.iddatatype == IT::CHAR) {
                    Out::WriteString(out, entry.id);
                    Out::WriteString(out, "_");
                    if(entry.scope == MAIN_SCOPE) Out::WriteString(out, "main");
                    else if(entry.scope == GLOBAL_SCOPE) Out::WriteString(out, "glob");
                    else Out::WriteString(out, idtable.table[entry.scope].id);
                    Out::WriteString(out, " BYTE '$', 0\n");
                } 
            }
        }

        Out::WriteString(out, "str_newline db 13, 10, 0\n");
        Out::WriteString(out, "random_seed dd 0\n");
        Out::WriteString(out, "console_handle dd 0\n");
        Out::WriteString(out, "bytes_written dd 0\n");
        Out::WriteString(out, "num_buffer db 32 dup(0)\n\n");

        Out::WriteString(out, "\n.code\n");
    }


    void AddFuncs(Out::OUT out) {
        // Генерация стандартных функций
        Out::WriteString(out, "; Стандартные функции\n");
        Out::WriteString(out, "pow proc a:dword, b:dword\n");
        Out::WriteString(out, "    push ebp\n");
        Out::WriteString(out, "    mov ebp, esp\n");
        Out::WriteString(out, "    mov eax, 1\n");
        Out::WriteString(out, "    mov ecx, [ebp+8]    ; a\n");
        Out::WriteString(out, "    mov edx, [ebp+12]   ; b\n");
        Out::WriteString(out, "    test edx, edx\n");
        Out::WriteString(out, "    jz pow_done\n");
        Out::WriteString(out, "pow_loop:\n");
        Out::WriteString(out, "    imul eax, ecx\n");
        Out::WriteString(out, "    dec edx\n");
        Out::WriteString(out, "    jnz pow_loop\n");
        Out::WriteString(out, "pow_done:\n");
        Out::WriteString(out, "    pop ebp\n");
        Out::WriteString(out, "    ret 8\n");
        Out::WriteString(out, "pow endp\n\n");

        Out::WriteString(out, "random proc min:dword, max:dword\n");
        Out::WriteString(out, "    push ebp\n");
        Out::WriteString(out, "    mov ebp, esp\n");
        Out::WriteString(out, "    push ebx\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Инициализация seed если нужно\n");
        Out::WriteString(out, "    cmp random_seed, 0\n");
        Out::WriteString(out, "    jne seed_ok\n");
        Out::WriteString(out, "    call GetTickCount\n");
        Out::WriteString(out, "    mov random_seed, eax\n");
        Out::WriteString(out, "seed_ok:\n");
        Out::WriteString(out, "    ; Линейный конгруэнтный генератор\n");
        Out::WriteString(out, "    mov eax, random_seed\n");
        Out::WriteString(out, "    mov ecx, 1103515245\n");
        Out::WriteString(out, "    mul ecx\n");
        Out::WriteString(out, "    add eax, 12345\n");
        Out::WriteString(out, "    mov random_seed, eax\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Получаем число в диапазоне\n");
        Out::WriteString(out, "    shr eax, 16         ; берем средние биты\n");
        Out::WriteString(out, "    and eax, 7FFFh\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    mov ecx, [ebp+12]    ; максимум\n");
        Out::WriteString(out, "    mov ebx, [ebp+8]     ; минимум\n");
        Out::WriteString(out, "    sub ecx, ebx\n");
        Out::WriteString(out, "    inc ecx\n");
        Out::WriteString(out, "    xor edx, edx\n");
        Out::WriteString(out, "    div ecx\n");
        Out::WriteString(out, "    mov eax, ebx\n");
        Out::WriteString(out, "    add eax, edx\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    pop ebx\n");
        Out::WriteString(out, "    pop ebp\n");
        Out::WriteString(out, "    ret 8\n");
        Out::WriteString(out, "random endp\n\n");

        // Вспомогательные функции для вывода
        Out::WriteString(out, "; Вспомогательные функции\n");
        Out::WriteString(out, "print_string proc strPtr:dword\n");
        Out::WriteString(out, "    push ebp\n");
        Out::WriteString(out, "    mov ebp, esp\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Вычисляем длину строки\n");
        Out::WriteString(out, "    mov esi, [ebp+8]\n");
        Out::WriteString(out, "    xor ecx, ecx\n");
        Out::WriteString(out, "strlen_loop:\n");
        Out::WriteString(out, "    cmp byte ptr [esi+ecx], 0\n");
        Out::WriteString(out, "    je strlen_done\n");
        Out::WriteString(out, "    inc ecx\n");
        Out::WriteString(out, "    jmp strlen_loop\n");
        Out::WriteString(out, "strlen_done:\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Выводим строку\n");
        Out::WriteString(out, "    invoke WriteConsoleA, console_handle, esi, ecx, offset bytes_written, 0\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    pop ebp\n");
        Out::WriteString(out, "    ret 4\n");
        Out::WriteString(out, "print_string endp\n\n");

        Out::WriteString(out, "print_uint proc num:dword\n");
        Out::WriteString(out, "    push ebp\n");
        Out::WriteString(out, "    mov ebp, esp\n");
        Out::WriteString(out, "    push ebx\n");
        Out::WriteString(out, "    push esi\n");
        Out::WriteString(out, "    push edi\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Преобразуем число в строку\n");
        Out::WriteString(out, "    mov eax, [ebp+8]\n");
        Out::WriteString(out, "    lea edi, [num_buffer+31]\n");
        Out::WriteString(out, "    mov byte ptr [edi], 0\n");
        Out::WriteString(out, "    dec edi\n");
        Out::WriteString(out, "    mov ebx, 10\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "convert_loop:\n");
        Out::WriteString(out, "    xor edx, edx\n");
        Out::WriteString(out, "    div ebx\n");
        Out::WriteString(out, "    add dl, '0'\n");
        Out::WriteString(out, "    mov [edi], dl\n");
        Out::WriteString(out, "    dec edi\n");
        Out::WriteString(out, "    test eax, eax\n");
        Out::WriteString(out, "    jnz convert_loop\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Выводим строку\n");
        Out::WriteString(out, "    lea eax, [edi+1]\n");
        Out::WriteString(out, "    push eax\n");
        Out::WriteString(out, "    call print_string\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    ; Выводим перевод строки\n");
        Out::WriteString(out, "    push offset str_newline\n");
        Out::WriteString(out, "    call print_string\n");
        Out::WriteString(out, "    \n");
        Out::WriteString(out, "    pop edi\n");
        Out::WriteString(out, "    pop esi\n");
        Out::WriteString(out, "    pop ebx\n");
        Out::WriteString(out, "    pop ebp\n");
        Out::WriteString(out, "    ret 4\n");
        Out::WriteString(out, "print_uint endp\n\n");
    }



    /*
    string processSWITCH(LT::LexTable lextable, IT::IdTable idtable, int start, int level) {
        int local_level = level;
        int caseNum = 0;
        string res = "";
        for (int i = 0; i < lextable.size; i++) {
            LT::Entry entry = lextable.table[i];
            char lexema = entry.lexema[0];

            switch (lexema) {
            case LEX_SWITCH:
                local_level += 1;
                break;
            case LEX_RIGHTBRACE:
                local_level -= 1;
                break;
            }

            if (local_level == level) {
                switch (lexema) {
                case LEX_CASE:
                    caseNum += 1;
                    i++;
					LT::Entry entry = lextable.table[i];
					IT::Entry info = idtable.table[entry.idxTI];
                    res += "cmp eax, ";
                    res += info.



                    cmp eax, 0
                        jne case0
                    break;  
                }

            }






        }





        return "";
    }


    */


    void Generate(LT::LexTable lextable, IT::IdTable idtable, Out::OUT out) {
        GenerateHeader(idtable, out);
        AddFuncs(out);
        Out::WriteString(out, "\n\n;=====================================\n\n");


        std::vector<int> scope;
        std::vector<int> caseNum;
        scope.push_back(GLOBAL_SCOPE);
        scope.push_back(0);
        int reserve = 0;


        for (int i = 0; i < lextable.size; i++) {
            LT::Entry entry = lextable.table[i];
            char lexema = entry.lexema[0];

            switch (lexema) {
            case LEX_FUNCTION: {
                i += 1;
                entry = lextable.table[i];
                scope.push_back(entry.idxTI);
                IT::Entry info = idtable.table[entry.idxTI];
                string funcName = info.id;

                Out::WriteString(out, funcName.c_str());
                Out::WriteString(out, " proc\n\n");

                while (lextable.table[i].lexema[0] != LEX_LEFTBRACE) {
                    i++;
                }

                break;
            }

            /*
            case LEX_PRINT: {
                i++; // Пропускаем PRINT

                // Ищем конец выражения для печати
                int start = i;
                int end = start;
                while (end < lextable.size && lextable.table[end].lexema[0] != LEX_SEMICOLON) {
                    end++;
                }

                // Генерируем код для печати
                string printExpr = PolishToASMExpression(lextable, idtable, start, end - 1);
                Out::WriteString(out, "    ; print\n");
                Out::WriteString(out, "    push ");
                Out::WriteString(out, printExpr.c_str());
                Out::WriteString(out, "\n");
                Out::WriteString(out, "    call print_uint\n");
                Out::WriteString(out, "    add esp, 4\n");

                i = end;
                break;
            }

            case LEX_RETURN: {
                i++; // Пропускаем RETURN

                // Ищем значение для возврата
                int start = i;
                int end = start;
                while (end < lextable.size && lextable.table[end].lexema[0] != LEX_SEMICOLON) {
                    end++;
                }

                string returnExpr = PolishToASMExpression(lextable, idtable, start, end - 1);
                Out::WriteString(out, "    mov eax, ");
                Out::WriteString(out, returnExpr.c_str());
                Out::WriteString(out, "\n");
                Out::WriteString(out, "    mov esp, ebp\n");
                Out::WriteString(out, "    pop ebp\n");
                Out::WriteString(out, "    ret\n");

                i = end;
                break;
            }
            */

            case LEX_SWITCH: {
                reserve += 1;
                caseNum.push_back(0);

                i += 2;

                int start = i;
                int end = start;
                while (end < lextable.size && lextable.table[end].lexema[0] != LEX_RIGHTHESIS) {
                    end++;
                }
                string switchExpr = PolishToASMExpression(lextable, idtable, start, end - 1);

                Out::WriteString(out, "    mov eax, ");
                Out::WriteString(out, switchExpr.c_str());
                Out::WriteString(out, "\n");



                i = end + 1; 
                break;
            }

            case LEX_CASE: {
                i++; 
                entry = lextable.table[i];
                IT::Entry caseInfo = idtable.table[entry.idxTI];

                string caseLabel = "case_" + to_string(caseInfo.value.vint);
                Out::WriteString(out, caseLabel.c_str());
                Out::WriteString(out, ":\n");

                i++; // Пропускаем значение case
                break;
            }

            case LEX_DEFAULT: {
                Out::WriteString(out, "default_case:\n");
                break;
            }

            case LEX_BREAK: {
                Out::WriteString(out, "\tjmp end_switch\n");
                break;
            }

            case LEX_RIGHTBRACE: {
                if(reserve > 0) Out::WriteString(out, "end_switch:\n");
                reserve -= 1;
                caseNum.pop_back();
                break;
            }

            case LEX_MAIN: {
                scope.push_back(MAIN_SCOPE);

                Out::WriteString(out, "main proc\n");
                i++; // Пропускаем MAIN
                Out::WriteString(out, "    invoke GetStdHandle, STD_OUTPUT_HANDLE\n");
                Out::WriteString(out, "    mov console_handle, eax\n\n");
                break;
            }
            }
        }

        Out::WriteString(out, "\tinvoke ExitProcess, 0\n");
        Out::WriteString(out, "main endp\n\n");
    }
}
