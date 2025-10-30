#pragma once
#include "FST.h"

namespace Automata {
    // ЗАРАНЕЕ заданные константные автоматы
    extern const FST::FST INTEGER;      // для "integer"
    extern const FST::FST STRING;       // для "string"  
    extern const FST::FST FUNCTION;     // для "function"
    extern const FST::FST DECLARE;      // для "declare"
    extern const FST::FST RETURN;       // для "return"
    extern const FST::FST PRINT;        // для "print"
    extern const FST::FST MAIN;         // для "main"
    extern const FST::FST IDENTIFIER;   // для идентификаторов [a-z]+

    // Автоматы для одиночных символов
    extern const FST::FST SEMICOLON;    // ;
    extern const FST::FST COMMA;        // ,
    extern const FST::FST LEFTPAREN;    // (
    extern const FST::FST RIGHTPAREN;   // )
    extern const FST::FST LEFTBRACE;    // {
    extern const FST::FST RIGHTBRACE;   // }
    extern const FST::FST ASSIGN;       // =
    extern const FST::FST PLUS;         // +
    extern const FST::FST MINUS;        // -
    extern const FST::FST STAR;         // *
    extern const FST::FST SLASH;        // /
}
