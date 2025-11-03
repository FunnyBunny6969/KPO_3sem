#pragma once
#include "FST.h"
#include "LT.h"

namespace Automata {

	char EMPTYstring[] = "";
	// ========== КЛЮЧЕВЫЕ СЛОВА ==========

	FST::FST INTEGER(
		(char*)"", 8,
		FST::NODE(1, FST::RELATION('i', 1)),
		FST::NODE(1, FST::RELATION('n', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('e', 4)),
		FST::NODE(1, FST::RELATION('g', 5)),
		FST::NODE(1, FST::RELATION('e', 6)),
		FST::NODE(1, FST::RELATION('r', 7)),
		FST::NODE()
	);

	FST::FST STRING(
		EMPTYstring, 7,
		FST::NODE(1, FST::RELATION('s', 1)),
		FST::NODE(1, FST::RELATION('t', 2)),
		FST::NODE(1, FST::RELATION('r', 3)),
		FST::NODE(1, FST::RELATION('i', 4)),
		FST::NODE(1, FST::RELATION('n', 5)),
		FST::NODE(1, FST::RELATION('g', 6)),
		FST::NODE()
	);

	FST::FST FUNCTION(
		EMPTYstring, 9,
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

	FST::FST DECLARE(
		EMPTYstring, 8,
		FST::NODE(1, FST::RELATION('d', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('c', 3)),
		FST::NODE(1, FST::RELATION('l', 4)),
		FST::NODE(1, FST::RELATION('a', 5)),
		FST::NODE(1, FST::RELATION('r', 6)),
		FST::NODE(1, FST::RELATION('e', 7)),
		FST::NODE()
	);

	FST::FST RETURN(
		EMPTYstring, 7,
		FST::NODE(1, FST::RELATION('r', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('t', 3)),
		FST::NODE(1, FST::RELATION('u', 4)),
		FST::NODE(1, FST::RELATION('r', 5)),
		FST::NODE(1, FST::RELATION('n', 6)),
		FST::NODE()
	);

	FST::FST PRINT(
		EMPTYstring, 6,
		FST::NODE(1, FST::RELATION('p', 1)),
		FST::NODE(1, FST::RELATION('r', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE(1, FST::RELATION('t', 5)),
		FST::NODE()
	);

	FST::FST MAIN(
		EMPTYstring, 5,
		FST::NODE(1, FST::RELATION('m', 1)),
		FST::NODE(1, FST::RELATION('a', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('n', 4)),
		FST::NODE()
	);

	// ========== ИДЕНТИФИКАТОРЫ И ЛИТЕРАЛЫ ==========

	FST::FST IDENTIFIER(
		EMPTYstring, 2,
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

	FST::FST NUMBER_LITERAL(
		EMPTYstring, 2,
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

	FST::FST STRING_LITERAL(
		EMPTYstring, 4,
		FST::NODE(1, FST::RELATION('\'', 1)),     // состояние 0: начальная кавычка
		FST::NODE(1, FST::RELATION('\'', 3)),     // состояние 1: сразу закрывающая кавычка (пустая строка)
		FST::NODE(2,                              // состояние 2: содержимое строки
			FST::RELATION('\'', 3),               // закрывающая кавычка -> конец
			FST::RELATION(0x00, 2)                // любой символ кроме ' -> остаемся в состоянии 2
		),
		FST::NODE()                               // состояние 3: конечное состояние
	);

	// ========== ОПЕРАТОРЫ И РАЗДЕЛИТЕЛИ ==========

	FST::FST SEMICOLON(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION(';', 1)),
		FST::NODE()
	);

	FST::FST COMMA(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION(',', 1)),
		FST::NODE()
	);

	FST::FST LEFTPAREN(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('(', 1)),
		FST::NODE()
	);

	FST::FST RIGHTPAREN(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION(')', 1)),
		FST::NODE()
	);

	FST::FST LEFTBRACE(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('{', 1)),
		FST::NODE()
	);

	FST::FST RIGHTBRACE(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('}', 1)),
		FST::NODE()
	);

	FST::FST ASSIGN(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('=', 1)),
		FST::NODE()
	);

	FST::FST PLUS(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('+', 1)),
		FST::NODE()
	);

	FST::FST MINUS(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('-', 1)),
		FST::NODE()
	);

	FST::FST STAR(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('*', 1)),
		FST::NODE()
	);

	FST::FST SLASH(
		EMPTYstring, 2,
		FST::NODE(1, FST::RELATION('/', 1)),
		FST::NODE()
	);

    // Вспомогательные функции для работы с автоматами
    bool executeAutomata(const FST::FST& automata, const char* text);  // выполнить автомат для текста
    char getLexemeCode(const char* text);                              // получить код лексемы для текста
}