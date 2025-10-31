#include "Automata.h"

namespace Automata {










	bool executeAutomata(const FST::FST& automata, const char* text) {
		FST::FST temp = automata;           // создаем копию автомата для безопасности
		temp.string = const_cast<char*>(text); // присваиваем текст для распознавания
		return FST::execute(temp);          // выполняем автомат и возвращаем результат
	}

    char getLexemeCode(const char* text) {
        // проверяем все автоматы в порядке приоритета
        if (executeAutomata(INTEGER, text)) return LEX_INTEGER;
        if (executeAutomata(STRING, text)) return LEX_STRING;
        if (executeAutomata(FUNCTION, text)) return LEX_FUNCTION;
        if (executeAutomata(DECLARE, text)) return LEX_DECLARE;
        if (executeAutomata(RETURN, text)) return LEX_RETURN;
        if (executeAutomata(PRINT, text)) return LEX_PRINT;
        if (executeAutomata(MAIN, text)) return 'm';  // специальный код для main

        if (executeAutomata(IDENTIFIER, text)) return LEX_ID;
        if (executeAutomata(NUMBER_LITERAL, text)) return LEX_LITERAL;
        if (executeAutomata(STRING_LITERAL, text)) return LEX_LITERAL;

        if (executeAutomata(SEMICOLON, text)) return LEX_SEMICOLON;
        if (executeAutomata(COMMA, text)) return LEX_COMMA;
        if (executeAutomata(LEFTPAREN, text)) return LEX_LEFTHESIS;
        if (executeAutomata(RIGHTPAREN, text)) return LEX_RIGHTHESIS;
        if (executeAutomata(LEFTBRACE, text)) return LEX_LEFTBRACE;
        if (executeAutomata(RIGHTBRACE, text)) return LEX_RIGHTBRACE;
        if (executeAutomata(ASSIGN, text)) return '=';
        if (executeAutomata(PLUS, text)) return LEX_PLUS;
        if (executeAutomata(MINUS, text)) return LEX_MINUS;
        if (executeAutomata(STAR, text)) return LEX_STAR;
        if (executeAutomata(SLASH, text)) return LEX_DIRSLASH;

        return '?'; // неизвестная лексема
    }
}








