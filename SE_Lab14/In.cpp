#include "In.h"
#include "Error.h"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

namespace In {

    // ������� �������� ��������
    static const int code_table[256] = IN_CODE_TABLE;

    IN getin(wchar_t infile[]) {
        IN result{};
        result.size = 0;
        result.lines = 1;  // �������� � 1 ������
        result.ignor = 0;

        // �������� ������ ��� ������
        result.text = new unsigned char[IN_MAX_LEN_TEXT];
        if (!result.text) {
            throw ERROR_THROW(110);
        }

        // ������������� ������� �������� ��������
        for (int i = 0; i < 256; i++) {
            result.code[i] = code_table[i];
        }

        std::ifstream file(infile, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            delete[] result.text;
            throw ERROR_THROW(110);
        }

        unsigned char ch;
        int pos_in_line = 1;  // �������� � ������� 1

        while (file.read(reinterpret_cast<char*>(&ch), 1)) {
            // �������� �� ������������ ������
            if (result.size >= IN_MAX_LEN_TEXT - 1) {
                break;
            }

            // ����� ������
            if (ch == IN_CODE_ENDL) {
                result.text[result.size++] = ch;
                result.lines++;
                pos_in_line = 1;  // ���������� ������� ��� ����� ������
                continue;
            }

            std::cout << ch<<"   "<<result.code<< std::endl;
            // �������� ������� �� �������
            int symbol_code = result.code[static_cast<unsigned char>(ch)];



            if (symbol_code == IN::F) {
                // ����������� ������
                delete[] result.text;
                throw ERROR_THROW_IN(111, result.lines, pos_in_line);
            }
            else if (symbol_code == IN::T) {
                // ������ ��������, ���������� ��� ����
                result.text[result.size++] = ch;
            }
            else if (symbol_code == IN::I) {
                // ���������� ������
                result.ignor++;
            }
            else {
                // ������ ������� (�������� �� 0 �� 255)
                result.text[result.size++] = static_cast<unsigned char>(symbol_code);
            }

            pos_in_line++;
        }

        // ��������� ������ ������� ��������
        result.text[result.size] = '\0';

        file.close();
        return result;
    }
}
