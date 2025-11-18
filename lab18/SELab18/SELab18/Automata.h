#pragma once
#include "FST.h"
#include "LT.h"
#include "string.h"

namespace Automata {
	// константные автоматы
	extern const FST::FST INTEGER;      // дл€ "integer"
	extern const FST::FST STRING;       // дл€ "string"  
	extern const FST::FST FUNCTION;     // дл€ "function"
	extern const FST::FST DECLARE;      // дл€ "declare"
	extern const FST::FST RETURN;       // дл€ "return"
	extern const FST::FST PRINT;        // дл€ "print"
	extern const FST::FST MAIN;         // дл€ "main"
	extern const FST::FST IDENTIFIER;   // дл€ идентификаторов [a-z]+

	// јвтоматы дл€ одиночных символов
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

	// јвтоматы дл€ литералов
	extern const FST::FST NUMBER_LITERAL;    // дл€ числовых литералов 123
	extern const FST::FST STRING_LITERAL;    // дл€ строковых литералов 'text'



    // ¬спомогательные функции дл€ работы с автоматами
    bool executeAutomata(const FST::FST& automata, const char* text);  // выполнить автомат дл€ текста
    char getLexemeCode(const char* text);                              // получить код лексемы дл€ текста
}