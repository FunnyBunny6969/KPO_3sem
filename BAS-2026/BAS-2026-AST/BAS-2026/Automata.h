#pragma once
#include "FST.h"
#include "LT.h"
#include "string.h"

namespace Automata {
    // константные автоматы
    extern const FST::FST SHORT;        // для "short"
    extern const FST::FST CHAR;         // для "char"
    extern const FST::FST STRING;       // для "string"  
    extern const FST::FST FUNCTION;     // для "function"
    extern const FST::FST RETURN;       // для "return"
    extern const FST::FST PRINT;        // для "print"
    extern const FST::FST GET;          // для "get"
    extern const FST::FST MAIN;         // для "main"
    extern const FST::FST BREAK;        // для "break"

    // Автоматы для одиночных символов
    extern const FST::FST SEMICOLON;    // ;
    extern const FST::FST COMMA;        // ,
    extern const FST::FST COLON;        // :
    extern const FST::FST LEFTPAREN;    // (
    extern const FST::FST RIGHTPAREN;   // )
    extern const FST::FST LEFTBRACE;    // {
    extern const FST::FST RIGHTBRACE;   // }

    extern const FST::FST COMP_OPERATOR; // ==, !=, <=, >=, <, > -> код 'c'
    extern const FST::FST ASSIGN;        // =
    extern const FST::FST PLUS;          // +
    extern const FST::FST MINUS;         // -
    extern const FST::FST STAR;          // *
    extern const FST::FST SLASH;         // /
    extern const FST::FST BIT_AND;       // && -> код '&'
    extern const FST::FST BIT_OR;        // || -> код '|'
    extern const FST::FST BIT_NOT;       // ~  -> код '~'

    // Идентификатор и литералы
    extern const FST::FST IDENTIFIER;
    extern const FST::FST HEX_NUMBER_LITERAL;
    extern const FST::FST NUMBER_LITERAL;
    extern const FST::FST CHAR_LITERAL;
    extern const FST::FST STRING_LITERAL;

    // Вспомогательные функции для работы с автоматами
    bool executeAutomata(const FST::FST& automata, const char* text);  // выполнить автомат для текста
    char getLexemeCode(const char* text);                              // получить код лексемы для текста
}