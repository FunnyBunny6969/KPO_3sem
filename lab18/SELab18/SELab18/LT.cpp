#include "LT.h"
#include <iostream>

namespace LT {

    // Создать таблицу лексем
    LexTable Create(int size) {
        if (size <= 0 || size > LT_MAXSIZE) {
            size = LT_MAXSIZE; // Используем максимальный размер по умолчанию
        }

        LexTable lextable;
        lextable.maxsize = size;
        lextable.size = 0;
        lextable.table = new Entry[size];

        // Инициализируем все записи
        for (int i = 0; i < size; i++) {
            lextable.table[i].lexema[0] = '\0';
            lextable.table[i].sn = 0;
            lextable.table[i].idxTI = LT_TI_NULLIDX;
        }

        return lextable;
    }

    // Добавить строку в таблицу лексем
    void Add(LexTable& lextable, Entry entry) {
        if (lextable.size >= lextable.maxsize) {
            throw std::runtime_error("Переполнение таблицы лексем");
        }

        // Копируем лексему (фиксированный размер 1 символ)
        lextable.table[lextable.size].lexema[0] = entry.lexema[0];
        lextable.table[lextable.size].lexema[1] = '\0'; // Завершающий ноль

        // Копируем остальные поля
        lextable.table[lextable.size].sn = entry.sn;
        lextable.table[lextable.size].idxTI = entry.idxTI;

        lextable.size++;
    }

    // Получить строку таблицы лексем
    Entry GetEntry(LexTable& lextable, int n) {
        if (n < 0 || n >= lextable.size) {
            Entry emptyEntry;
            emptyEntry.lexema[0] = '\0';
            emptyEntry.sn = -1;
            emptyEntry.idxTI = LT_TI_NULLIDX;
            return emptyEntry;
        }

        return lextable.table[n];
    }

    // Удалить таблицу лексем (освободить память)
    void Delete(LexTable& lextable) {
        if (lextable.table != nullptr) {
            delete[] lextable.table;
            lextable.table = nullptr;
        }
        lextable.size = 0;
        lextable.maxsize = 0;
    }
}