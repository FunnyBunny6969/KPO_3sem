#pragma once

#include "LT.h" // Предполагаем, что LT::LexTable и LT::Entry здесь
#include "IT.h" // Предполагаем, что IT::IdTable и IT::Entry здесь
#include <string>
#include <map>
#include <stack>
#include <vector>

namespace PN {

    // Лексемы, которые необходимо определить в ваших заголовочных файлах (LT.h)
    // #define LEX_STAR        '*'
    // #define LEX_DIRSLASH    '/'
    // #define LEX_PLUS        '+'
    // #define LEX_MINUS       '-'
    // #define LEX_LEFTHESIS   '('
    // #define LEX_RIGHTHESIS  ')'
    // #define LEX_SEMICOLON   ';'
    // #define LEX_COMMA       ','
    // #define LEX_EQUALS      '='
    // #define LEX_ID          'i'
    // #define LEX_LITERAL     'l'
    // #define LEX_RETURN      'r'
    // #define LEX_PRINT       'p'
    // #define LEX_GRT         '>'  // Больше
    // #define LEX_LESS        '<'  // Меньше
    // #define LEX_EQ          'q'  // ==
    // #define LEX_NEQ         'n'  // !=
    // #define LEX_GEQ         'g'  // >=
    // #define LEX_LEQ         'e'  // <=


    // Ищет выражения после операторов присваивания, print и return
    void FindExpressions(LT::LexTable& lextable, IT::IdTable& idtable);

    // Главная функция преобразования выражения в ОПЗ (Обратную Польскую Запись)
    bool PolishNotation(int start_pos, LT::LexTable& lextable, IT::IdTable& idtable);
}
