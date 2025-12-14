#pragma once

#define LEXEMA_FIXSIZE 1           // фиксированный размер лексемы
#define LT_MAXSIZE 4096            // максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX 0xffffffff   // нет элемента таблицы идентификаторов

#define LEX_UINT        't'   // лексема для unsigned_integer
#define LEX_STRING      't'   // лексема для string
#define LEX_CHAR        't'   // лексема для char
#define LEX_ID          'i'   // лексема для идентификатора
#define LEX_LITERAL     'l'   // лексема для литерала
#define LEX_FUNCTION    'f'   // лексема для function
#define LEX_DECLARE     'd'   // лексема для declare
#define LEX_RETURN      'r'   // лексема для return
#define LEX_PRINT       'p'   // лексема для print
#define LEX_SEMICOLON   ';'   // лексема для ;
#define LEX_COMMA       ','   // лексема для ,
#define LEX_LEFTBRACE   '{'   // лексема для {
#define LEX_RIGHTBRACE  '}'   // лексема для }
#define LEX_LEFTHESIS   '('   // лексема для (
#define LEX_RIGHTHESIS  ')'   // лексема для )
#define LEX_PLUS        '+'   // лексема для +
#define LEX_MINUS       '-'   // лексема для -
#define LEX_STAR        '*'   // лексема для *
#define LEX_DIRSLASH    '/'   // лексема для /

#define LEX_EQUALS      '='
#define LEX_MAIN        'm'
#define LEX_UNKNOWN     '?'

#define LEX_SWITCH      'w'
#define LEX_CASE        'k'
#define LEX_DEFAULT     'j'
#define LEX_COLON       ':'

#define LEX_BIT_AND     '&'
#define LEX_BIT_OR      '|'
#define LEX_BIT_NOT     '~'



namespace LT    // таблица лексем
{
    struct Entry        // строка таблицы лексем
    {
        char lexema[LEXEMA_FIXSIZE];  // лексема
        int sn;                       // номер строки в исходном тексте
        int idxTI;                    // индекс в таблице идентификаторов или LT_TI_NULLIDX
    };

    struct LexTable     // экземпляр таблицы лексем
    {
        int maxsize;                  // емкость таблицы лексем < LT_MAXSIZE
        int size;                     // текущий размер таблицы лексем < maxsize
        Entry* table;                 // массив строк таблицы лексем
    };

    LexTable Create(    // создать таблицу лексем
					int size          // емкость таблицы лексем < LT_MAXSIZE
					);

    void Add(           // добавить строку в таблицу лексем
			LexTable& lextable,       // экземпляр таблицы лексем
			Entry entry               // строка таблицы лексем
			);

    Entry GetEntry(     // получить строку таблицы лексем
				LexTable& lextable,   // экземпляр таблицы лексем
				int n                 // номер получаемой строки
				);

    void Delete(LexTable& lextable);  // удалить таблицу лексем (освободить память)
};




