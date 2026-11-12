#pragma once

#define LEXEMA_FIXSIZE  2           // 1 символ лексемы + завершающий '\0'
#define LT_MAXSIZE      4096        // максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX   0xffffffff  // нет элемента таблицы идентификаторов

//  оды лексем €зыка BAS-2026
#define LEX_SHORT       't'         // short
#define LEX_STRING      't'         // string
#define LEX_CHAR        't'         // char
#define LEX_ID          'i'         // идентификатор
#define LEX_LITERAL     'l'         // литерал
#define LEX_FUNCTION    'f'         // function
#define LEX_RETURN      'r'         // return
#define LEX_PRINT       'p'         // print
#define LEX_GET         'g'         // get (ввод)
#define LEX_MAIN        'm'         // main
#define LEX_WHILE       'w'         // while
#define LEX_BREAK       'b'         // break
#define LEX_SEMICOLON   ';'         // ;
#define LEX_COMMA       ','         // ,
#define LEX_COLON       ':'         // :
#define LEX_LEFTBRACE   '{'         // {
#define LEX_RIGHTBRACE  '}'         // }
#define LEX_LEFTHESIS   '('         // (
#define LEX_RIGHTHESIS  ')'         // )
#define LEX_PLUS        '+'         // +
#define LEX_MINUS       '-'         // -
#define LEX_STAR        '*'         // *
#define LEX_DIRSLASH    '/'         // /
#define LEX_BIT_AND     '&'         // &&
#define LEX_BIT_OR      '|'         // ||
#define LEX_BIT_NOT     '~'         // ~
#define LEX_COMP        'c'         // ==, !=, <, >, <=, >=
#define LEX_EQUALS      '='         // =
#define LEX_UNKNOWN     '?'         // неизвестна€ лексема



namespace LT    // таблица лексем
{
    struct Entry        // строка таблицы лексем
    {
        char lexema[LEXEMA_FIXSIZE];  // лексема
        int sn;                       // номер строки в исходном тексте
        int idxTI;                    // индекс в таблице идентификаторов или LT_TI_NULLIDX
        char op[4]{};
    };

    struct LexTable     // экземпл€р таблицы лексем
    {
        int maxsize;                  // емкость таблицы лексем < LT_MAXSIZE
        int size;                     // текущий размер таблицы лексем < maxsize
        Entry* table;                 // массив строк таблицы лексем
    };

    LexTable Create(    // создать таблицу лексем
					int size          // емкость таблицы лексем < LT_MAXSIZE
					);

    void Add(           // добавить строку в таблицу лексем
			LexTable& lextable,       // экземпл€р таблицы лексем
			Entry entry               // строка таблицы лексем
			);

    Entry GetEntry(     // получить строку таблицы лексем
				LexTable& lextable,   // экземпл€р таблицы лексем
				int n                 // номер получаемой строки
				);

    void Delete(LexTable& lextable);  // удалить таблицу лексем (освободить пам€ть)
};




