#include "In.h"
#include "Error.h"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;

namespace In {

    // Таблица проверки символов
    static const int code_table[256] = IN_CODE_TABLE;

    IN getin(wchar_t infile[]) {
        IN result{};
        result.size = 0;
        result.lines = 0;
        result.ignor = 0;

        // Выделяем память для текста
        result.text = new unsigned char[IN_MAX_LEN_TEXT];
        if (!result.text) {
            throw ERROR_THROW(110);
        }

        // Инициализация таблицы проверки символов
        for (int i = 0; i < 256; i++) {
            result.code[i] = code_table[i];
        }

        std::ifstream file(infile, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            delete[] result.text;
            throw ERROR_THROW(110);
        }

        unsigned char ch;
        int pos_in_line = 0;

        while (file.read(reinterpret_cast<char*>(&ch), 1)) {
            // Конец строки
            cout << ch << endl;
            if (ch == IN_CODE_ENDL) {
                result.text[result.size++] = ch;
                result.lines++;
                pos_in_line = 0;
                continue;
            }

            // Проверка символа
            if (result.code[ch] == IN::F) {
                delete[] result.text;
                throw ERROR_THROW_IN(111, result.lines, pos_in_line);
            }
            else if (result.code[ch] == IN::T) {
                // Символ разрешен, записываем
                result.text[result.size++] = ch;
            }
            else if (result.code[ch] == IN::I) {
                // Игнорируем символ
                result.ignor++;
            }
            else {
                // Если в таблице другой код (0–255), заменяем на него
                result.text[result.size++] = static_cast<unsigned char>(result.code[ch]);
            }

            pos_in_line++;
            if (result.size >= IN_MAX_LEN_TEXT) break; // защита от переполнения
        }

        // Если файл не пустой, но не заканчивается \n
        if (result.size > 0 && result.text[result.size - 1] != IN_CODE_ENDL) {
            result.lines++;
        }

        file.close();
        return result;
    }
}
