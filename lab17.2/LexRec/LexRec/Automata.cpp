#include "Automata.h"

namespace Automata {
	// ========== КЛЮЧЕВЫЕ СЛОВА ==========
	const FST::FST INTEGER = FST::FST(
		"", 8,
		FST::NODE(1, FST::RELATION('i', 1)),
		FST::NODE(1, FST::RELATION('n', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('e', 4)),
		FST::NODE(1, FST::RELATION('g', 5)),
		FST::NODE(1, FST::RELATION('e', 6)),
		FST::NODE(1, FST::RELATION('r', 7)),
		FST::NODE()
	);

	const FST::FST STRING = FST::FST(
		"", 7,
		FST::NODE(1, FST::RELATION('s', 1)),
		FST::NODE(1, FST::RELATION('t', 2)),
		FST::NODE(1, FST::RELATION('r', 3)),
		FST::NODE(1, FST::RELATION('i', 4)),
		FST::NODE(1, FST::RELATION('n', 5)),
		FST::NODE(1, FST::RELATION('g', 6)),
		FST::NODE()
	);

	const FST::FST FUNCTION = FST::FST(
		"", 9,
		FST::NODE(1, FST::RELATION('f', 1)),
		FST::NODE(1, FST::RELATION('u', 2)),
		FST::NODE(1, FST::RELATION('n', 3)),
		FST::NODE(1, FST::RELATION('c', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE(1, FST::RELATION('i', 6)),
		FST::NODE(1, FST::RELATION('o', 7)),
		FST::NODE(1, FST::RELATION('n', 8)),
		FST::NODE()
	);

	const FST::FST DECLARE = FST::FST(
		"", 8,
		FST::NODE(1, FST::RELATION('d', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('c', 3)),
		FST::NODE(1, FST::RELATION('l', 4)),
		FST::NODE(1, FST::RELATION('a', 5)),
		FST::NODE(1, FST::RELATION('r', 6)),
		FST::NODE(1, FST::RELATION('e', 7)),
		FST::NODE()
	);

	const FST::FST RETURN = FST::FST (
		"", 7,
		FST::NODE(1, FST::RELATION('r', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('u', 4)),
		FST::NODE(1, FST::RELATION('r', 5)),
		FST::NODE(1, FST::RELATION('n', 6)),
		FST::NODE()
	);

	const FST::FST PRINT = FST::FST(
		"", 6,
		FST::NODE(1, FST::RELATION('p', 1)),
		FST::NODE(1, FST::RELATION('r', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE()
	);

	const FST::FST MAIN = FST::FST(
		"", 5,
		FST::NODE(1, FST::RELATION('m', 1)),
		FST::NODE(1, FST::RELATION('a', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE()
	);

	// ========== ИДЕНТИФИКАТОРЫ И ЛИТЕРАЛЫ ==========

	const FST::FST IDENTIFIER = FST::FST(
		"", 2,
		FST::NODE(26,  // начальное состояние: только буквы
			FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1),
			FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
			FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1),
			FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
			FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
			FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
			FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1),
			FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
			FST::RELATION('y', 1), FST::RELATION('z', 1)
		),
		FST::NODE(37,  // последующие состояния: буквы, цифры, подчеркивание
			FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1),
			FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
			FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1),
			FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
			FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
			FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
			FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1),
			FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
			FST::RELATION('y', 1), FST::RELATION('z', 1),
			FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
			FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
			FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
			FST::RELATION('9', 1), FST::RELATION('_', 1)
		)
	);

	const FST::FST NUMBER_LITERAL = FST::FST(
		"", 2,
		FST::NODE(10,  // начальное состояние: только цифры
			FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
			FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
			FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
			FST::RELATION('9', 1)
		),
		FST::NODE(10,  // последующие состояния: только цифры
			FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
			FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
			FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
			FST::RELATION('9', 1)
		)
	);

	const FST::FST STRING_LITERAL = FST::FST(
		"", 3,
		FST::NODE(1, FST::RELATION('\'', 1)),     
		FST::NODE(2,                             
			FST::RELATION('\'', 2),             
			FST::RELATION(0x20, 1)             
		),
		FST::NODE()                           
	);

	// ========== ОПЕРАТОРЫ И РАЗДЕЛИТЕЛИ ==========

	const FST::FST SEMICOLON = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION(';', 1)),
		FST::NODE()
	);

	const FST::FST COMMA = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION(',', 1)),
		FST::NODE()
	);

	const FST::FST LEFTPAREN = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('(', 1)),
		FST::NODE()
	);

	const FST::FST RIGHTPAREN = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION(')', 1)),
		FST::NODE()
	);

	const FST::FST LEFTBRACE = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('{', 1)),
		FST::NODE()
	);

	const FST::FST RIGHTBRACE = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('}', 1)),
		FST::NODE()
	);

	const FST::FST ASSIGN = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('=', 1)),
		FST::NODE()
	);

	const FST::FST PLUS = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('+', 1)),
		FST::NODE()
	);

	const FST::FST MINUS = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('-', 1)),
		FST::NODE()
	);

	const FST::FST STAR = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('*', 1)),
		FST::NODE()
	);

	const FST::FST SLASH = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('/', 1)),
		FST::NODE()
	);




	bool executeAutomata(const FST::FST& automata, const char* text) {
		FST::FST tempAutomata(text, automata.nstates, automata.nodes[0]);

		for (short i = 1; i < automata.nstates; i++) {
			tempAutomata.nodes[i] = automata.nodes[i];
		}

		bool result = FST::execute(tempAutomata);

		// Дополнительная проверка: убедиться что распознана ВСЯ строка
		// и достигнуто конечное состояние
		if (result) {
			// Проверяем что последнее состояние достигнуто и позиция = длина строки
			return (tempAutomata.rstates[tempAutomata.nstates - 1] == strlen(text));
		}

		return result;
	}


	char getLexemeCode(const char* text) {
		if (executeAutomata(INTEGER, text)) return LEX_INTEGER;
		if (executeAutomata(STRING, text)) return LEX_STRING;
		if (executeAutomata(FUNCTION, text)) return LEX_FUNCTION;
		if (executeAutomata(DECLARE, text)) return LEX_DECLARE;
		if (executeAutomata(RETURN, text)) return LEX_RETURN;
		if (executeAutomata(PRINT, text)) return LEX_PRINT;
		if (executeAutomata(MAIN, text)) return 'm';  

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

		return '?'; 
	}
}








