#include "pch.h"
#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cstring>
#include <cctype>

extern "C" {

    void __stdcall _std_print_short(short val) {
        std::cout << val << std::endl;
    }

    void __stdcall _std_print_char(unsigned int ch) {
        std::cout << static_cast<char>(ch & 0xFF) << std::endl;
    }

    void __stdcall _std_print_str(const char* str) {
        if (str != nullptr) {
            std::cout << str << std::endl;
        }
    }

    short __stdcall _std_input_short() {
        int temp = 0;
        std::cin >> temp;
        return static_cast<short>(temp);
    }

    unsigned int __stdcall _std_input_char() {
        char ch = 0;
        std::cin >> ch;
        return static_cast<unsigned char>(ch);
    }

    short __stdcall _std_pow(short base, short exp) {
        return static_cast<short>(std::pow(base, exp));
    }

    const char* __stdcall _std_itos(short val) {
        static char buffers[16][32];
        static int idx = 0;

        char* buf = buffers[idx];
        idx = (idx + 1) % 16;

        snprintf(buf, 32, "%d", static_cast<int>(val));
        return buf;
    }

    const char* __stdcall _std_topolish(const char* expr) {
        static char outBuf[256];
        if (expr == nullptr) {
            outBuf[0] = '\0';
            return outBuf;
        }

        auto priority = [](char op) -> int {
            if (op == '~') return 4;
            if (op == '*' || op == '/') return 3;
            if (op == '+' || op == '-') return 2;
            if (op == '&' || op == '|') return 1;
            return 0;
            };

        std::string res;
        std::stack<char> ops;

        for (size_t i = 0; expr[i] != '\0'; ++i) {
            char c = expr[i];
            if (c == ' ' || c == '\t') continue;

            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
                while (std::isalnum(static_cast<unsigned char>(expr[i])) || expr[i] == '_') {
                    res += expr[i++];
                }
                res += ' ';
                --i;
            }
            else if (c == '(') {
                ops.push(c);
            }
            else if (c == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    res += ops.top();
                    res += ' ';
                    ops.pop();
                }
                if (!ops.empty()) ops.pop();
            }
            else {
                while (!ops.empty() && priority(ops.top()) >= priority(c)) {
                    res += ops.top();
                    res += ' ';
                    ops.pop();
                }
                ops.push(c);
            }
        }

        while (!ops.empty()) {
            if (ops.top() != '(') {
                res += ops.top();
                res += ' ';
            }
            ops.pop();
        }

        strncpy_s(outBuf, sizeof(outBuf), res.c_str(), sizeof(outBuf) - 1);
        return outBuf;
    }
}
