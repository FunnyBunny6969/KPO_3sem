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
        result.lines = 1;  // Начинаем с 1 строки
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
        int pos_in_line = 1;  // Начинаем с позиции 1

        while (file.read(reinterpret_cast<char*>(&ch), 1)) {
            // Проверка на переполнение буфера
            if (result.size >= IN_MAX_LEN_TEXT - 1) {
                break;
            }

            // Конец строки
            if (ch == IN_CODE_ENDL) {
                result.text[result.size++] = ch;
                result.lines++;
                pos_in_line = 1;  // Сбрасываем позицию для новой строки
                continue;
            }

            std::cout << ch<<"   "<<result.code<< std::endl;
            // Проверка символа по таблице
            int symbol_code = result.code[static_cast<unsigned char>(ch)];



            if (symbol_code == IN::F) {
                // Запрещенный символ
                delete[] result.text;
                throw ERROR_THROW_IN(111, result.lines, pos_in_line);
            }
            else if (symbol_code == IN::T) {
                // Символ разрешен, записываем как есть
                result.text[result.size++] = ch;
            }
            else if (symbol_code == IN::I) {
                // Игнорируем символ
                result.ignor++;
            }
            else {
                // Замена символа (значение от 0 до 255)
                result.text[result.size++] = static_cast<unsigned char>(symbol_code);
            }

            pos_in_line++;
        }

        // Завершаем строку нулевым символом
        result.text[result.size] = '\0';

        file.close();
        return result;
    }
}
