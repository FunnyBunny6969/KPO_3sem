#pragma once

#define IT_MAX_PARAMS 8
#define ID_MAXSIZE  5              // максимальное количество символов в идентификаторе
#define ID_REALSIZE 20
#define TI_MAXSIZE  4096           // максимальное количество строк в таблице идентификаторов
#define TI_INT_DEFAULT 0x00000000  // значение по умолчанию дл€ типа integer
#define TI_STR_DEFAULT 0x00        // значение по умолчанию дл€ типа string
#define TI_NULLIDX  0xffffffff     // нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE 255

namespace IT    // таблица идентификаторов
{
    enum IDDATATYPE {INT=1, STR=2, BOOL=3, UNDEF = 0};   // типы данных идентификаторов: integer, string
    enum IDTYPE     {V=1, F=2, P=3, L=4};                // типы идентификаторов: переменна€, функци€, параметр, литерал

    struct Entry    // строка таблицы идентификаторов
    {
        int        idxfirstLE;        // индекс первой строки в таблице лексем
        char       id[ID_REALSIZE];    // идентификатор (автоматически усекаетс€ до ID_MAXSIZE)
        IDDATATYPE iddatatype;        // тип данных
        IDTYPE     idtype;            // тип идентификатора

        union
        {
            int vint;                          // значение integer
            struct
            {
                char len;                      // количество символов в string
                char str[TI_STR_MAXSIZE - 1];  // символы string
            } vstr[TI_STR_MAXSIZE];            // значение string
        } value;                       // значение идентификатора

        struct
        {
            int          n_params = 0;                // „исло параметров 
            IDDATATYPE   params_types[IT_MAX_PARAMS]; // ћассив типов данных параметров
        } func_meta;
    };

    struct IdTable        // экземпл€р таблицы идентификаторов
    {
        int    maxsize;   // емкость таблицы идентификаторов < TI_MAXSIZE
        int    size;      // текущий размер таблицы идентификаторов < maxsize
        Entry* table;     // массив строк таблицы идентификаторов
    };

    IdTable Create(       // создать таблицу идентификаторов
				int size  // емкость таблицы идентификаторов < TI_MAXSIZE
				);

    void Add(                   // добавить строку в таблицу идентификаторов
			IdTable& idtable,   // экземпл€р таблицы идентификаторов
			Entry entry         // строка таблицы идентификаторов
			);

    Entry GetEntry(                // получить строку таблицы идентификаторов
				IdTable& idtable,  // экземпл€р таблицы идентификаторов
				int n              // номер получаемой строки
				);

    int IsId(                    // возврат: номер строки (если есть), TI_NULLIDX(если нет)
			IdTable& idtable,    // экземпл€р таблицы идентификаторов
			char id[ID_MAXSIZE]  // идентификатор
			);

    void Delete(IdTable& idtable);    // удалить таблицу идентификаторов (освободить пам€ть)

    void InitBuiltins(IdTable& idTable);
}


