#include "Automata.h"

namespace Automata {
	// ========== КЛЮЧЕВЫЕ СЛОВА ==========
	const FST::FST U_INTEGER = FST::FST(
		"", 17,
		FST::NODE(1, FST::RELATION('u', 1)),
		FST::NODE(1, FST::RELATION('n', 2)),
		FST::NODE(1, FST::RELATION('s', 3)),
		FST::NODE(1, FST::RELATION('i', 4)),
		FST::NODE(1, FST::RELATION('g', 5)),
		FST::NODE(1, FST::RELATION('n', 6)),
		FST::NODE(1, FST::RELATION('e', 7)),
		FST::NODE(1, FST::RELATION('d', 8)),
		FST::NODE(1, FST::RELATION('_', 9)),
		FST::NODE(1, FST::RELATION('i', 10)),
		FST::NODE(1, FST::RELATION('n', 11)),
		FST::NODE(1, FST::RELATION('t', 12)),
		FST::NODE(1, FST::RELATION('e', 13)),
		FST::NODE(1, FST::RELATION('g', 14)),
		FST::NODE(1, FST::RELATION('e', 15)),
		FST::NODE(1, FST::RELATION('r', 16)),
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

	const FST::FST CHAR = FST::FST(
		"", 5,
		FST::NODE(1, FST::RELATION('c', 1)),
		FST::NODE(1, FST::RELATION('h', 2)),
		FST::NODE(1, FST::RELATION('a', 3)),
		FST::NODE(1, FST::RELATION('r', 4)),
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

	const FST::FST SWITCH = FST::FST(
		"", 7,
		FST::NODE(1, FST::RELATION('s', 1)),
		FST::NODE(1, FST::RELATION('w', 2)),
		FST::NODE(1, FST::RELATION('i', 3)),
		FST::NODE(1, FST::RELATION('t', 4)),
		FST::NODE(1, FST::RELATION('c', 5)),
		FST::NODE(1, FST::RELATION('h', 6)),
		FST::NODE()
	);

	const FST::FST CASE = FST::FST(
		"", 5,
		FST::NODE(1, FST::RELATION('c', 1)),
		FST::NODE(1, FST::RELATION('a', 2)),
		FST::NODE(1, FST::RELATION('s', 3)),
		FST::NODE(1, FST::RELATION('e', 4)),
		FST::NODE()
	);

	const FST::FST DEFAULT = FST::FST(
		"", 8,
		FST::NODE(1, FST::RELATION('d', 1)),
		FST::NODE(1, FST::RELATION('e', 2)),
		FST::NODE(1, FST::RELATION('f', 3)),
		FST::NODE(1, FST::RELATION('a', 4)),
		FST::NODE(1, FST::RELATION('u', 5)),
		FST::NODE(1, FST::RELATION('l', 6)),
		FST::NODE(1, FST::RELATION('t', 7)),
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



	const FST::FST HEX_NUMBER_LITERAL = FST::FST(
		"", 4,
		FST::NODE(1, FST::RELATION('0', 1)),
		FST::NODE(1, FST::RELATION('x', 2)),
		FST::NODE(22,  // начальное состояние: только цифры
			FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3),
			FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3),
			FST::RELATION('6', 3), FST::RELATION('7', 3), FST::RELATION('8', 3),
			FST::RELATION('9', 3), FST::RELATION('A', 3), FST::RELATION('B', 3), 
			FST::RELATION('C', 3), FST::RELATION('D', 3), FST::RELATION('E', 3), 
			FST::RELATION('F', 3), FST::RELATION('a', 3), FST::RELATION('b', 3),
			FST::RELATION('c', 3), FST::RELATION('d', 3), FST::RELATION('e', 3), 
			FST::RELATION('f', 3)
		),
		FST::NODE(22,
			FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3),
			FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3),
			FST::RELATION('6', 3), FST::RELATION('7', 3), FST::RELATION('8', 3),
			FST::RELATION('9', 3), FST::RELATION('A', 3), FST::RELATION('B', 3), 
			FST::RELATION('C', 3), FST::RELATION('D', 3), FST::RELATION('E', 3), 
			FST::RELATION('F', 3), FST::RELATION('a', 3), FST::RELATION('b', 3),
			FST::RELATION('c', 3), FST::RELATION('d', 3), FST::RELATION('e', 3), 
			FST::RELATION('f', 3)
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

	const FST::FST CHAR_LITERAL = FST::FST(
		"", 4,
		FST::NODE(1, FST::RELATION('\'', 1)),     
		FST::NODE(255,                           
			// Символы 0x01-0x1F (управляющие)
			FST::RELATION(0x01, 2), FST::RELATION(0x02, 2), FST::RELATION(0x03, 2),
			FST::RELATION(0x04, 2), FST::RELATION(0x05, 2), FST::RELATION(0x06, 2),
			FST::RELATION(0x07, 2), FST::RELATION(0x08, 2), FST::RELATION(0x09, 2),
			FST::RELATION(0x0A, 2), FST::RELATION(0x0B, 2), FST::RELATION(0x0C, 2),
			FST::RELATION(0x0D, 2), FST::RELATION(0x0E, 2), FST::RELATION(0x0F, 2),
			FST::RELATION(0x10, 2), FST::RELATION(0x11, 2), FST::RELATION(0x12, 2),
			FST::RELATION(0x13, 2), FST::RELATION(0x14, 2), FST::RELATION(0x15, 2),
			FST::RELATION(0x16, 2), FST::RELATION(0x17, 2), FST::RELATION(0x18, 2),
			FST::RELATION(0x19, 2), FST::RELATION(0x1A, 2), FST::RELATION(0x1B, 2),
			FST::RELATION(0x1C, 2), FST::RELATION(0x1D, 2), FST::RELATION(0x1E, 2),
			FST::RELATION(0x1F, 2),

			// Печатные символы 0x20-0x7E
			FST::RELATION(' ', 2), FST::RELATION('!', 2), FST::RELATION('"', 2),
			FST::RELATION('#', 2), FST::RELATION('$', 2), FST::RELATION('%', 2),
			FST::RELATION('&', 2), FST::RELATION('(', 2), FST::RELATION(')', 2),
			FST::RELATION('*', 2), FST::RELATION('+', 2), FST::RELATION(',', 2),
			FST::RELATION('-', 2), FST::RELATION('.', 2), FST::RELATION('/', 2),
			FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2),
			FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2),
			FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2),
			FST::RELATION('9', 2), FST::RELATION(':', 2), FST::RELATION(';', 2),
			FST::RELATION('<', 2), FST::RELATION('=', 2), FST::RELATION('>', 2),
			FST::RELATION('?', 2), FST::RELATION('@', 2), FST::RELATION('A', 2),
			FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2),
			FST::RELATION('E', 2), FST::RELATION('F', 2), FST::RELATION('G', 2),
			FST::RELATION('H', 2), FST::RELATION('I', 2), FST::RELATION('J', 2),
			FST::RELATION('K', 2), FST::RELATION('L', 2), FST::RELATION('M', 2),
			FST::RELATION('N', 2), FST::RELATION('O', 2), FST::RELATION('P', 2),
			FST::RELATION('Q', 2), FST::RELATION('R', 2), FST::RELATION('S', 2),
			FST::RELATION('T', 2), FST::RELATION('U', 2), FST::RELATION('V', 2),
			FST::RELATION('W', 2), FST::RELATION('X', 2), FST::RELATION('Y', 2),
			FST::RELATION('Z', 2), FST::RELATION('[', 2), FST::RELATION('\\', 2),
			FST::RELATION(']', 2), FST::RELATION('^', 2), FST::RELATION('_', 2),
			FST::RELATION('`', 2), FST::RELATION('a', 2), FST::RELATION('b', 2),
			FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2),
			FST::RELATION('f', 2), FST::RELATION('g', 2), FST::RELATION('h', 2),
			FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2),
			FST::RELATION('l', 2), FST::RELATION('m', 2), FST::RELATION('n', 2),
			FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2),
			FST::RELATION('r', 2), FST::RELATION('s', 2), FST::RELATION('t', 2),
			FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2),
			FST::RELATION('x', 2), FST::RELATION('y', 2), FST::RELATION('z', 2),
			FST::RELATION('{', 2), FST::RELATION('|', 2), FST::RELATION('}', 2),
			FST::RELATION('~', 2),

			// Символ 0x7F (DEL)
			FST::RELATION(0x7F, 2),

			// Кириллица Win1251 0x80-0xFF
			FST::RELATION(0x80, 2), // Ђ
			FST::RELATION(0x81, 2), // Ѓ
			FST::RELATION(0x82, 2), // ‚
			FST::RELATION(0x83, 2), // ѓ
			FST::RELATION(0x84, 2), // „
			FST::RELATION(0x85, 2), // …
			FST::RELATION(0x86, 2), // †
			FST::RELATION(0x87, 2), // ‡
			FST::RELATION(0x88, 2), // €
			FST::RELATION(0x89, 2), // ‰
			FST::RELATION(0x8A, 2), // Љ
			FST::RELATION(0x8B, 2), // ‹
			FST::RELATION(0x8C, 2), // Њ
			FST::RELATION(0x8D, 2), // Ќ
			FST::RELATION(0x8E, 2), // Ћ
			FST::RELATION(0x8F, 2), // Џ
			FST::RELATION(0x90, 2), // ђ
			FST::RELATION(0x91, 2), // ‘
			FST::RELATION(0x92, 2), // ’
			FST::RELATION(0x93, 2), // “
			FST::RELATION(0x94, 2), // ”
			FST::RELATION(0x95, 2), // •
			FST::RELATION(0x96, 2), // –
			FST::RELATION(0x97, 2), // —
			FST::RELATION(0x98, 2), // 
			FST::RELATION(0x99, 2), // ™
			FST::RELATION(0x9A, 2), // љ
			FST::RELATION(0x9B, 2), // ›
			FST::RELATION(0x9C, 2), // њ
			FST::RELATION(0x9D, 2), // ќ
			FST::RELATION(0x9E, 2), // ћ
			FST::RELATION(0x9F, 2), // џ
			FST::RELATION(0xA0, 2), // NO-BREAK SPACE
			FST::RELATION(0xA1, 2), // Ў
			FST::RELATION(0xA2, 2), // ў
			FST::RELATION(0xA3, 2), // Ј
			FST::RELATION(0xA4, 2), // ¤
			FST::RELATION(0xA5, 2), // Ґ
			FST::RELATION(0xA6, 2), // ¦
			FST::RELATION(0xA7, 2), // §
			FST::RELATION(0xA8, 2), // Ё
			FST::RELATION(0xA9, 2), // ©
			FST::RELATION(0xAA, 2), // Є
			FST::RELATION(0xAB, 2), // «
			FST::RELATION(0xAC, 2), // ¬
			FST::RELATION(0xAD, 2), // SHY
			FST::RELATION(0xAE, 2), // ®
			FST::RELATION(0xAF, 2), // Ї
			FST::RELATION(0xB0, 2), // °
			FST::RELATION(0xB1, 2), // ±
			FST::RELATION(0xB2, 2), // І
			FST::RELATION(0xB3, 2), // і
			FST::RELATION(0xB4, 2), // ґ
			FST::RELATION(0xB5, 2), // µ
			FST::RELATION(0xB6, 2), // ¶
			FST::RELATION(0xB7, 2), // ·
			FST::RELATION(0xB8, 2), // ё
			FST::RELATION(0xB9, 2), // №
			FST::RELATION(0xBA, 2), // є
			FST::RELATION(0xBB, 2), // »
			FST::RELATION(0xBC, 2), // ј
			FST::RELATION(0xBD, 2), // Ѕ
			FST::RELATION(0xBE, 2), // ѕ
			FST::RELATION(0xBF, 2), // ї
			FST::RELATION(0xC0, 2), // А
			FST::RELATION(0xC1, 2), // Б
			FST::RELATION(0xC2, 2), // В
			FST::RELATION(0xC3, 2), // Г
			FST::RELATION(0xC4, 2), // Д
			FST::RELATION(0xC5, 2), // Е
			FST::RELATION(0xC6, 2), // Ж
			FST::RELATION(0xC7, 2), // З
			FST::RELATION(0xC8, 2), // И
			FST::RELATION(0xC9, 2), // Й
			FST::RELATION(0xCA, 2), // К
			FST::RELATION(0xCB, 2), // Л
			FST::RELATION(0xCC, 2), // М
			FST::RELATION(0xCD, 2), // Н
			FST::RELATION(0xCE, 2), // О
			FST::RELATION(0xCF, 2), // П
			FST::RELATION(0xD0, 2), // Р
			FST::RELATION(0xD1, 2), // С
			FST::RELATION(0xD2, 2), // Т
			FST::RELATION(0xD3, 2), // У
			FST::RELATION(0xD4, 2), // Ф
			FST::RELATION(0xD5, 2), // Х
			FST::RELATION(0xD6, 2), // Ц
			FST::RELATION(0xD7, 2), // Ч
			FST::RELATION(0xD8, 2), // Ш
			FST::RELATION(0xD9, 2), // Щ
			FST::RELATION(0xDA, 2), // Ъ
			FST::RELATION(0xDB, 2), // Ы
			FST::RELATION(0xDC, 2), // Ь
			FST::RELATION(0xDD, 2), // Э
			FST::RELATION(0xDE, 2), // Ю
			FST::RELATION(0xDF, 2), // Я
			FST::RELATION(0xE0, 2), // а
			FST::RELATION(0xE1, 2), // б
			FST::RELATION(0xE2, 2), // в
			FST::RELATION(0xE3, 2), // г
			FST::RELATION(0xE4, 2), // д
			FST::RELATION(0xE5, 2), // е
			FST::RELATION(0xE6, 2), // ж
			FST::RELATION(0xE7, 2), // з
			FST::RELATION(0xE8, 2), // и
			FST::RELATION(0xE9, 2), // й
			FST::RELATION(0xEA, 2), // к
			FST::RELATION(0xEB, 2), // л
			FST::RELATION(0xEC, 2), // м
			FST::RELATION(0xED, 2), // н
			FST::RELATION(0xEE, 2), // о
			FST::RELATION(0xEF, 2), // п
			FST::RELATION(0xF0, 2), // р
			FST::RELATION(0xF1, 2), // с
			FST::RELATION(0xF2, 2), // т
			FST::RELATION(0xF3, 2), // у
			FST::RELATION(0xF4, 2), // ф
			FST::RELATION(0xF5, 2), // х
			FST::RELATION(0xF6, 2), // ц
			FST::RELATION(0xF7, 2), // ч
			FST::RELATION(0xF8, 2), // ш
			FST::RELATION(0xF9, 2), // щ
			FST::RELATION(0xFA, 2), // ъ
			FST::RELATION(0xFB, 2), // ы
			FST::RELATION(0xFC, 2), // ь
			FST::RELATION(0xFD, 2), // э
			FST::RELATION(0xFE, 2), // ю
			FST::RELATION(0xFF, 2)  // я
		),
		FST::NODE(1, FST::RELATION('\'', 3)),     
		FST::NODE()                              
	);

	const FST::FST STRING_LITERAL = FST::FST(
		"", 3,
		FST::NODE(1, FST::RELATION('\'', 1)),     
		FST::NODE(256,                           
			// Символы 0x01-0x1F (управляющие)
			FST::RELATION(0x01, 1), FST::RELATION(0x02, 1), FST::RELATION(0x03, 1),
			FST::RELATION(0x04, 1), FST::RELATION(0x05, 1), FST::RELATION(0x06, 1),
			FST::RELATION(0x07, 1), FST::RELATION(0x08, 1), FST::RELATION(0x09, 1),
			FST::RELATION(0x0A, 1), FST::RELATION(0x0B, 1), FST::RELATION(0x0C, 1),
			FST::RELATION(0x0D, 1), FST::RELATION(0x0E, 1), FST::RELATION(0x0F, 1),
			FST::RELATION(0x10, 1), FST::RELATION(0x11, 1), FST::RELATION(0x12, 1),
			FST::RELATION(0x13, 1), FST::RELATION(0x14, 1), FST::RELATION(0x15, 1),
			FST::RELATION(0x16, 1), FST::RELATION(0x17, 1), FST::RELATION(0x18, 1),
			FST::RELATION(0x19, 1), FST::RELATION(0x1A, 1), FST::RELATION(0x1B, 1),
			FST::RELATION(0x1C, 1), FST::RELATION(0x1D, 1), FST::RELATION(0x1E, 1),
			FST::RELATION(0x1F, 1),

			// Печатные символы 0x20-0x7E
			FST::RELATION(' ', 1), FST::RELATION('!', 1), FST::RELATION('"', 1),
			FST::RELATION('#', 1), FST::RELATION('$', 1), FST::RELATION('%', 1),
			FST::RELATION('&', 1), FST::RELATION('(', 1), FST::RELATION(')', 1),
			FST::RELATION('*', 1), FST::RELATION('+', 1), FST::RELATION(',', 1),
			FST::RELATION('-', 1), FST::RELATION('.', 1), FST::RELATION('/', 1),
			FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
			FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
			FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
			FST::RELATION('9', 1), FST::RELATION(':', 1), FST::RELATION(';', 1),
			FST::RELATION('<', 1), FST::RELATION('=', 1), FST::RELATION('>', 1),
			FST::RELATION('?', 1), FST::RELATION('@', 1), FST::RELATION('A', 1),
			FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1),
			FST::RELATION('E', 1), FST::RELATION('F', 1), FST::RELATION('G', 1),
			FST::RELATION('H', 1), FST::RELATION('I', 1), FST::RELATION('J', 1),
			FST::RELATION('K', 1), FST::RELATION('L', 1), FST::RELATION('M', 1),
			FST::RELATION('N', 1), FST::RELATION('O', 1), FST::RELATION('P', 1),
			FST::RELATION('Q', 1), FST::RELATION('R', 1), FST::RELATION('S', 1),
			FST::RELATION('T', 1), FST::RELATION('U', 1), FST::RELATION('V', 1),
			FST::RELATION('W', 1), FST::RELATION('X', 1), FST::RELATION('Y', 1),
			FST::RELATION('Z', 1), FST::RELATION('[', 1), FST::RELATION('\\', 1),
			FST::RELATION(']', 1), FST::RELATION('^', 1), FST::RELATION('_', 1),
			FST::RELATION('`', 1), FST::RELATION('a', 1), FST::RELATION('b', 1),
			FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
			FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1),
			FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1),
			FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1),
			FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1),
			FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1),
			FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1),
			FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1),
			FST::RELATION('{', 1), FST::RELATION('|', 1), FST::RELATION('}', 1),
			FST::RELATION('~', 1),

			// Символ 0x7F (DEL)
			FST::RELATION(0x7F, 1),

			// Кириллица Win1251 0x80-0xFF
			FST::RELATION(0x80, 1), // Ђ
			FST::RELATION(0x81, 1), // Ѓ
			FST::RELATION(0x82, 1), // ‚
			FST::RELATION(0x83, 1), // ѓ
			FST::RELATION(0x84, 1), // „
			FST::RELATION(0x85, 1), // …
			FST::RELATION(0x86, 1), // †
			FST::RELATION(0x87, 1), // ‡
			FST::RELATION(0x88, 1), // €
			FST::RELATION(0x89, 1), // ‰
			FST::RELATION(0x8A, 1), // Љ
			FST::RELATION(0x8B, 1), // ‹
			FST::RELATION(0x8C, 1), // Њ
			FST::RELATION(0x8D, 1), // Ќ
			FST::RELATION(0x8E, 1), // Ћ
			FST::RELATION(0x8F, 1), // Џ
			FST::RELATION(0x90, 1), // ђ
			FST::RELATION(0x91, 1), // ‘
			FST::RELATION(0x92, 1), // ’
			FST::RELATION(0x93, 1), // “
			FST::RELATION(0x94, 1), // ”
			FST::RELATION(0x95, 1), // •
			FST::RELATION(0x96, 1), // –
			FST::RELATION(0x97, 1), // —
			FST::RELATION(0x98, 1), // 
			FST::RELATION(0x99, 1), // ™
			FST::RELATION(0x9A, 1), // љ
			FST::RELATION(0x9B, 1), // ›
			FST::RELATION(0x9C, 1), // њ
			FST::RELATION(0x9D, 1), // ќ
			FST::RELATION(0x9E, 1), // ћ
			FST::RELATION(0x9F, 1), // џ
			FST::RELATION(0xA0, 1), // NO-BREAK SPACE
			FST::RELATION(0xA1, 1), // Ў
			FST::RELATION(0xA2, 1), // ў
			FST::RELATION(0xA3, 1), // Ј
			FST::RELATION(0xA4, 1), // ¤
			FST::RELATION(0xA5, 1), // Ґ
			FST::RELATION(0xA6, 1), // ¦
			FST::RELATION(0xA7, 1), // §
			FST::RELATION(0xA8, 1), // Ё
			FST::RELATION(0xA9, 1), // ©
			FST::RELATION(0xAA, 1), // Є
			FST::RELATION(0xAB, 1), // «
			FST::RELATION(0xAC, 1), // ¬
			FST::RELATION(0xAD, 1), // SHY
			FST::RELATION(0xAE, 1), // ®
			FST::RELATION(0xAF, 1), // Ї
			FST::RELATION(0xB0, 1), // °
			FST::RELATION(0xB1, 1), // ±
			FST::RELATION(0xB2, 1), // І
			FST::RELATION(0xB3, 1), // і
			FST::RELATION(0xB4, 1), // ґ
			FST::RELATION(0xB5, 1), // µ
			FST::RELATION(0xB6, 1), // ¶
			FST::RELATION(0xB7, 1), // ·
			FST::RELATION(0xB8, 1), // ё
			FST::RELATION(0xB9, 1), // №
			FST::RELATION(0xBA, 1), // є
			FST::RELATION(0xBB, 1), // »
			FST::RELATION(0xBC, 1), // ј
			FST::RELATION(0xBD, 1), // Ѕ
			FST::RELATION(0xBE, 1), // ѕ
			FST::RELATION(0xBF, 1), // ї
			FST::RELATION(0xC0, 1), // А
			FST::RELATION(0xC1, 1), // Б
			FST::RELATION(0xC2, 1), // В
			FST::RELATION(0xC3, 1), // Г
			FST::RELATION(0xC4, 1), // Д
			FST::RELATION(0xC5, 1), // Е
			FST::RELATION(0xC6, 1), // Ж
			FST::RELATION(0xC7, 1), // З
			FST::RELATION(0xC8, 1), // И
			FST::RELATION(0xC9, 1), // Й
			FST::RELATION(0xCA, 1), // К
			FST::RELATION(0xCB, 1), // Л
			FST::RELATION(0xCC, 1), // М
			FST::RELATION(0xCD, 1), // Н
			FST::RELATION(0xCE, 1), // О
			FST::RELATION(0xCF, 1), // П
			FST::RELATION(0xD0, 1), // Р
			FST::RELATION(0xD1, 1), // С
			FST::RELATION(0xD2, 1), // Т
			FST::RELATION(0xD3, 1), // У
			FST::RELATION(0xD4, 1), // Ф
			FST::RELATION(0xD5, 1), // Х
			FST::RELATION(0xD6, 1), // Ц
			FST::RELATION(0xD7, 1), // Ч
			FST::RELATION(0xD8, 1), // Ш
			FST::RELATION(0xD9, 1), // Щ
			FST::RELATION(0xDA, 1), // Ъ
			FST::RELATION(0xDB, 1), // Ы
			FST::RELATION(0xDC, 1), // Ь
			FST::RELATION(0xDD, 1), // Э
			FST::RELATION(0xDE, 1), // Ю
			FST::RELATION(0xDF, 1), // Я
			FST::RELATION(0xE0, 1), // а
			FST::RELATION(0xE1, 1), // б
			FST::RELATION(0xE2, 1), // в
			FST::RELATION(0xE3, 1), // г
			FST::RELATION(0xE4, 1), // д
			FST::RELATION(0xE5, 1), // е
			FST::RELATION(0xE6, 1), // ж
			FST::RELATION(0xE7, 1), // з
			FST::RELATION(0xE8, 1), // и
			FST::RELATION(0xE9, 1), // й
			FST::RELATION(0xEA, 1), // к
			FST::RELATION(0xEB, 1), // л
			FST::RELATION(0xEC, 1), // м
			FST::RELATION(0xED, 1), // н
			FST::RELATION(0xEE, 1), // о
			FST::RELATION(0xEF, 1), // п
			FST::RELATION(0xF0, 1), // р
			FST::RELATION(0xF1, 1), // с
			FST::RELATION(0xF2, 1), // т
			FST::RELATION(0xF3, 1), // у
			FST::RELATION(0xF4, 1), // ф
			FST::RELATION(0xF5, 1), // х
			FST::RELATION(0xF6, 1), // ц
			FST::RELATION(0xF7, 1), // ч
			FST::RELATION(0xF8, 1), // ш
			FST::RELATION(0xF9, 1), // щ
			FST::RELATION(0xFA, 1), // ъ
			FST::RELATION(0xFB, 1), // ы
			FST::RELATION(0xFC, 1), // ь
			FST::RELATION(0xFD, 1), // э
			FST::RELATION(0xFE, 1), // ю
			FST::RELATION(0xFF, 1), // я

			FST::RELATION('\'', 2)                
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

	const FST::FST COLON = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION(':', 1)),
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

	const FST::FST BIT_AND = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('&', 1)),
		FST::NODE()
	);

	const FST::FST BIT_OR = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('|', 1)),
		FST::NODE()
	);

	const FST::FST BIT_NOT = FST::FST(
		"", 2,
		FST::NODE(1, FST::RELATION('~', 1)),
		FST::NODE()
	);


	bool executeAutomata(const FST::FST& automata, const char* text) {
		FST::FST tempAutomata(text, automata.nstates, automata.nodes[0]);

		for (short i = 1; i < automata.nstates; i++) {
			tempAutomata.nodes[i] = automata.nodes[i];
		}

		return FST::execute(tempAutomata);
	}


	char getLexemeCode(const char* text) {
		if (executeAutomata(U_INTEGER, text)) return LEX_UINT;
		if (executeAutomata(CHAR, text)) return LEX_CHAR;
		if (executeAutomata(STRING, text)) return LEX_STRING;
		if (executeAutomata(FUNCTION, text)) return LEX_FUNCTION;
		if (executeAutomata(DECLARE, text)) return LEX_DECLARE;
		if (executeAutomata(RETURN, text)) return LEX_RETURN;
		if (executeAutomata(PRINT, text)) return LEX_PRINT;
		if (executeAutomata(MAIN, text)) return LEX_MAIN;  
		if (executeAutomata(SWITCH, text)) return LEX_SWITCH;  
		if (executeAutomata(CASE, text)) return LEX_CASE;  
		if (executeAutomata(DEFAULT, text)) return LEX_DEFAULT;  


		if (executeAutomata(IDENTIFIER, text)) return LEX_ID;

		if (executeAutomata(NUMBER_LITERAL, text)) return LEX_LITERAL;
		if (executeAutomata(CHAR_LITERAL, text)) return LEX_LITERAL;
		if (executeAutomata(STRING_LITERAL, text)) return LEX_LITERAL;
		if (executeAutomata(HEX_NUMBER_LITERAL, text)) return LEX_LITERAL;

		if (executeAutomata(SEMICOLON, text)) return LEX_SEMICOLON;
		if (executeAutomata(COLON, text)) return LEX_COLON;
		if (executeAutomata(COMMA, text)) return LEX_COMMA;
		if (executeAutomata(LEFTPAREN, text)) return LEX_LEFTHESIS;
		if (executeAutomata(RIGHTPAREN, text)) return LEX_RIGHTHESIS;
		if (executeAutomata(LEFTBRACE, text)) return LEX_LEFTBRACE;
		if (executeAutomata(RIGHTBRACE, text)) return LEX_RIGHTBRACE;
		if (executeAutomata(ASSIGN, text)) return LEX_EQUALS;
		if (executeAutomata(PLUS, text)) return LEX_PLUS;
		if (executeAutomata(MINUS, text)) return LEX_MINUS;
		if (executeAutomata(STAR, text)) return LEX_STAR;
		if (executeAutomata(SLASH, text)) return LEX_DIRSLASH;
		if (executeAutomata(BIT_AND, text)) return LEX_BIT_AND;
		if (executeAutomata(BIT_OR, text)) return LEX_BIT_OR;
		if (executeAutomata(BIT_NOT, text)) return LEX_BIT_NOT;

		return LEX_UNKNOWN; 
	}
}








