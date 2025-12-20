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
            idtable.table[i].iddatatype = UINT;
            idtable.table[i].idtype = V;

            // Инициализируем значения по умолчанию
            idtable.table[i].value.vint = TI_INT_DEFAULT;
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
        if (entry.iddatatype == UINT) {
            idtable.table[idtable.size].value.vint = entry.value.vint;
        }
        else if (entry.iddatatype == CHAR) {
            idtable.table[idtable.size].value.vchar = entry.value.vchar;
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
        idtable.table[idtable.size].scope = entry.scope;

        idtable.size++;
    }

    // Получить строку таблицы идентификаторов
    Entry GetEntry(IdTable& idtable, int n) {
        if (n < 0 || n >= idtable.size) {
            Entry emptyEntry;
            emptyEntry.idxfirstLE = -1;
            emptyEntry.id[0] = '\0';
            emptyEntry.iddatatype = UINT;
            emptyEntry.idtype = V;
            emptyEntry.value.vint = TI_INT_DEFAULT;
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
            if (idtable.table[i].id == id && 
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
        IT::Entry powEntry;
        strncpy_s(powEntry.id, ID_REALSIZE, "pow", _TRUNCATE);
        powEntry.id[ID_REALSIZE - 1] = '\0';
        powEntry.idtype = IT::F;  
        powEntry.iddatatype = IT::UINT;   
        powEntry.idxfirstLE = -1;     
        powEntry.scope = GLOBAL_SCOPE;
        powEntry.func_meta.n_params = 2;
        powEntry.func_meta.params_types[0] = IT::UINT;
        powEntry.func_meta.params_types[1] = IT::UINT;
        IT::Add(idTable, powEntry);

        IT::Entry randEntry;
        strncpy_s(randEntry.id, ID_REALSIZE, "random", _TRUNCATE);
        randEntry.id[ID_REALSIZE - 1] = '\0';
        randEntry.idtype = IT::F; 
        randEntry.iddatatype = IT::UINT; 
        randEntry.idxfirstLE = -1;     
        randEntry.scope = GLOBAL_SCOPE;
        randEntry.func_meta.n_params = 0;  
        IT::Add(idTable, randEntry);

        IT::Entry strlenEntry;
        strncpy_s(strlenEntry.id, ID_REALSIZE, "strlen", _TRUNCATE);
        strlenEntry.id[ID_REALSIZE - 1] = '\0';
        strlenEntry.idtype = IT::F;  
        strlenEntry.iddatatype = IT::UINT;     
        strlenEntry.idxfirstLE = -1;          
        strlenEntry.scope = GLOBAL_SCOPE;
        strlenEntry.func_meta.n_params = 1;  
        strlenEntry.func_meta.params_types[0] = IT::STR; 
        IT::Add(idTable, strlenEntry);
    }
}