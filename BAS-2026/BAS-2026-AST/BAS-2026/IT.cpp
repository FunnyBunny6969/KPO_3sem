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
            idtable.table[i].iddatatype = SHORT;
            idtable.table[i].idtype = V;

            // Инициализируем значения по умолчанию
            idtable.table[i].value.vshort = TI_INT_DEFAULT;
        }

        return idtable;
    }

    // Добавить строку в таблицу идентификаторов
    void Add(IdTable& idtable, Entry entry, bool needToCheck) {
        if (idtable.size >= idtable.maxsize) {
            throw std::runtime_error("Переполнение таблицы идентификаторов");
        }

        // Проверяем, нет ли уже такого идентификатора
        if (needToCheck) {
			int existingIndex = IsId(idtable, entry.id, entry.scope);
			if (existingIndex != TI_NULLIDX) {
				// Идентификатор уже существует - обновляем запись
				idtable.table[existingIndex] = entry;
				return;
			}
        }

        // Копируем идентификатор (усекаем до ID_MAXSIZE)
        strncpy_s(idtable.table[idtable.size].id, ID_REALSIZE, entry.id, _TRUNCATE);

        // Копируем остальные поля
        idtable.table[idtable.size].idxfirstLE = entry.idxfirstLE;
        idtable.table[idtable.size].iddatatype = entry.iddatatype;
        idtable.table[idtable.size].idtype = entry.idtype;

        // Копируем значение в зависимости от типа данных
        if (entry.iddatatype == SHORT) {
            idtable.table[idtable.size].value.vshort = entry.value.vshort;
        }
        else if (entry.iddatatype == CHAR) {
            idtable.table[idtable.size].value.vchar = entry.value.vchar;
        }
        else if (entry.iddatatype == STR) {
            // Копируем строковое значение
            idtable.table[idtable.size].value.vstr.len = entry.value.vstr.len;
            strncpy_s(idtable.table[idtable.size].value.vstr.str,
                ID_REALSIZE - 1,
                entry.value.vstr.str,
                _TRUNCATE);
        }

        if (entry.idtype == IT::F) {
            idtable.table[idtable.size].func_meta = entry.func_meta;
        }
        idtable.table[idtable.size].scope = entry.scope;

        idtable.size++;
    }

    // Получить строку таблицы идентификаторов
    Entry GetEntry(IdTable& idtable, int n) {
        if (n < 0 || n >= idtable.size) {
            Entry emptyEntry;
            emptyEntry.idxfirstLE = -1;
            emptyEntry.id[0] = '\0';
            emptyEntry.iddatatype = SHORT;
            emptyEntry.idtype = V;
            emptyEntry.value.vshort = TI_INT_DEFAULT;
            return emptyEntry;
        }

        return idtable.table[n];
    }

    // Проверить наличие идентификатора в таблице
    int IsId(IdTable& idtable, char id[ID_REALSIZE], int scope) {
        for (int i = 0; i < idtable.size; i++) {
            
            //VAR
            if (strcmp(idtable.table[i].id, id) == 0 &&
                (idtable.table[i].scope == scope ||
                    idtable.table[i].scope == GLOBAL_SCOPE) &&
                (idtable.table[i].idtype == IT::V ||
                idtable.table[i].idtype == IT::P)
                ) {
                return i; 
            }

            // FUNC
            if (strcmp(idtable.table[i].id, id) == 0 &&
                idtable.table[i].idtype == IT::F &&
                (idtable.table[i].scope == scope ||
                    idtable.table[i].scope == GLOBAL_SCOPE)) {
                return i; 
            }

            //LITERAL
            if (strcmp(idtable.table[i].id, id) == 0 &&
                idtable.table[i].idtype == IT::L)
                return i;
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
        // 1. pow(short, short) -> short
        IT::Entry powEntry{};
        strncpy_s(powEntry.id, ID_REALSIZE, "pow", _TRUNCATE);
        powEntry.idtype = IT::F;
        powEntry.iddatatype = IT::SHORT;
        powEntry.idxfirstLE = -1;
        powEntry.scope = GLOBAL_SCOPE;
        powEntry.func_meta.n_params = 2;
        powEntry.func_meta.params_types[0] = IT::SHORT;
        powEntry.func_meta.params_types[1] = IT::SHORT;
        IT::Add(idTable, powEntry, false);

        // 2. itos(short) -> string
        IT::Entry itosEntry{};
        strncpy_s(itosEntry.id, ID_REALSIZE, "itos", _TRUNCATE);
        itosEntry.idtype = IT::F;
        itosEntry.iddatatype = IT::STR;
        itosEntry.idxfirstLE = -1;
        itosEntry.scope = GLOBAL_SCOPE;
        itosEntry.func_meta.n_params = 1;
        itosEntry.func_meta.params_types[0] = IT::SHORT;
        IT::Add(idTable, itosEntry, false);

        // 3. topolish(string) -> string
        IT::Entry rpnEntry{};
        strncpy_s(rpnEntry.id, ID_REALSIZE, "topolish", _TRUNCATE);
        rpnEntry.idtype = IT::F;
        rpnEntry.iddatatype = IT::STR;
        rpnEntry.idxfirstLE = -1;
        rpnEntry.scope = GLOBAL_SCOPE;
        rpnEntry.func_meta.n_params = 1;
        rpnEntry.func_meta.params_types[0] = IT::STR;
        IT::Add(idTable, rpnEntry, false);
    }

}