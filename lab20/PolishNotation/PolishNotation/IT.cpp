#include "IT.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace IT {

    // Создать таблицу идентификаторов
    IdTable Create(int size) {
        if (size <= 0 || size > TI_MAXSIZE) {
            size = TI_MAXSIZE; // Используем максимальный размер по умолчанию
        }

        IdTable idtable;
        idtable.maxsize = size;
        idtable.size = 0;
        idtable.table = new Entry[size];

        // Инициализируем все записи
        for (int i = 0; i < size; i++) {
            idtable.table[i].idxfirstLE = -1;
            idtable.table[i].id[0] = '\0';
            idtable.table[i].iddatatype = INT;
            idtable.table[i].idtype = V;

            // Инициализируем значения по умолчанию
            idtable.table[i].value.vint = TI_INT_DEFAULT;
        }

        return idtable;
    }

    // Добавить строку в таблицу идентификаторов
    void Add(IdTable& idtable, Entry entry) {
        if (idtable.size >= idtable.maxsize) {
            throw std::runtime_error("Переполнение таблицы идентификаторов");
        }

        // Проверяем, нет ли уже такого идентификатора
        int existingIndex = IsId(idtable, entry.id);
        if (existingIndex != TI_NULLIDX) {
            // Идентификатор уже существует - обновляем запись
            idtable.table[existingIndex] = entry;
            return;
        }

        // Копируем идентификатор (усекаем до ID_MAXSIZE)
        strncpy_s(idtable.table[idtable.size].id, ID_REALSIZE, entry.id, _TRUNCATE);

        // Копируем остальные поля
        idtable.table[idtable.size].idxfirstLE = entry.idxfirstLE;
        idtable.table[idtable.size].iddatatype = entry.iddatatype;
        idtable.table[idtable.size].idtype = entry.idtype;

        // Копируем значение в зависимости от типа данных
        if (entry.iddatatype == INT) {
            idtable.table[idtable.size].value.vint = entry.value.vint;
        }
        else if (entry.iddatatype == STR) {
            // Копируем строковое значение
            idtable.table[idtable.size].value.vstr[0].len = entry.value.vstr[0].len;
            strncpy_s(idtable.table[idtable.size].value.vstr[0].str,
                ID_REALSIZE - 1,
                entry.value.vstr[0].str,
                _TRUNCATE);
        }

        if (entry.idtype == IT::F) {
            idtable.table[idtable.size].func_meta = entry.func_meta;
        }

        idtable.size++;
    }

    // Получить строку таблицы идентификаторов
    Entry GetEntry(IdTable& idtable, int n) {
        if (n < 0 || n >= idtable.size) {
            Entry emptyEntry;
            emptyEntry.idxfirstLE = -1;
            emptyEntry.id[0] = '\0';
            emptyEntry.iddatatype = INT;
            emptyEntry.idtype = V;
            emptyEntry.value.vint = TI_INT_DEFAULT;
            return emptyEntry;
        }

        return idtable.table[n];
    }

    // Проверить наличие идентификатора в таблице
    int IsId(IdTable& idtable, char id[ID_REALSIZE]) {
        for (int i = 0; i < idtable.size; i++) {
            if (strcmp(idtable.table[i].id, id) == 0) {
                return i; // Найден, возвращаем индекс
            }
        }
        return TI_NULLIDX; // Не найден
    }

    // Удалить таблицу идентификаторов (освободить память)
    void Delete(IdTable& idtable) {
        if (idtable.table != nullptr) {
            delete[] idtable.table;
            idtable.table = nullptr;
        }
        idtable.size = 0;
        idtable.maxsize = 0;
    }

    void InitBuiltins(IdTable& idTable) {
        IT::Entry strlenEntry;
        strncpy_s(strlenEntry.id, ID_REALSIZE, "strlen", _TRUNCATE);
        strlenEntry.id[ID_REALSIZE - 1] = '\0';
        strlenEntry.idtype = IT::F;           // Тип: Функция
        strlenEntry.iddatatype = IT::INT;     // Возвращаемый тип: integer
        strlenEntry.idxfirstLE = -1;          // Индекс лексемы: -1 (встроенная функция)
        strlenEntry.func_meta.n_params = 1;                           // Арность: 1
        strlenEntry.func_meta.params_types[0] = IT::STR;              // Тип первого параметра: string
        IT::Add(idTable, strlenEntry);

        IT::Entry substrEntry;
        strncpy_s(substrEntry.id, ID_REALSIZE, "substr", _TRUNCATE);
        substrEntry.id[ID_REALSIZE - 1] = '\0';
        substrEntry.idtype = IT::F;           
        substrEntry.iddatatype = IT::STR;    
        substrEntry.idxfirstLE = -1;        
        substrEntry.func_meta.n_params = 3;                           
        substrEntry.func_meta.params_types[0] = IT::STR;             
        substrEntry.func_meta.params_types[1] = IT::INT;            
        substrEntry.func_meta.params_types[2] = IT::INT;           
        IT::Add(idTable, substrEntry);
    }
}