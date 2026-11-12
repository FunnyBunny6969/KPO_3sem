#pragma once

#include "GRB.h"
#define GRB_ERROR_SERIES 600

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'),         // стартовый символ, дно стека
		6,                                      // количество правил

		// 1. Правило S — структура программы
		Rule(NS('S'), GRB_ERROR_SERIES + 0,     // Неверная структура программы
			6,
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), TS(';')),
			Rule::Chain(6, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),

			Rule::Chain(9, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS(';')),
			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),

			Rule::Chain(10, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';')),
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S'))
		),

		// 2. Правило N — операторы внутри блока (поддерживает вложенные циклы while и ввод get)
		Rule(NS('N'), GRB_ERROR_SERIES + 1,     // Ошибочный оператор
			20,

			// Объявление переменной: short a;
			Rule::Chain(3, TS('t'), TS('i'), TS(';')),
			Rule::Chain(4, TS('t'), TS('i'), TS(';'), NS('N')),

			// Ввод переменной: get a;
			Rule::Chain(3, TS('g'), TS('i'), TS(';')),
			Rule::Chain(4, TS('g'), TS('i'), TS(';'), NS('N')),

			// Присваивание: a = E;
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),

			// Вызов функции как отдельный оператор: foo(); или foo(W);
			Rule::Chain(4, TS('i'), TS('('), TS(')'), TS(';')),
			Rule::Chain(5, TS('i'), TS('('), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),

			// Вывод: print E;
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),

			// Возврат: return E;
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),

			// Цикл WHILE: while (E) { N };
			Rule::Chain(8, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';')),
			Rule::Chain(9, TS('w'), TS('('), NS('E'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('N')),

			// Оператор break;
			Rule::Chain(2, TS('b'), TS(';')),
			Rule::Chain(3, TS('b'), TS(';'), NS('N')),

			// Вызов функции в начале выражения-оператора
			Rule::Chain(5, TS('i'), TS('('), TS(')'), NS('M'), TS(';')),
			Rule::Chain(6, TS('i'), TS('('), TS(')'), NS('M'), TS(';'), NS('N'))
		),

		// 3. Правило E — выражения (переменные, литералы, вызовы функций, скобки, унарные '-' и '~')
		Rule(NS('E'), GRB_ERROR_SERIES + 2,     // Ошибка в выражении
			14,

			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),

			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),

			Rule::Chain(1, TS('i')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('l'), NS('M')),

			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),

			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),

			Rule::Chain(2, TS('~'), NS('E')),
			Rule::Chain(3, TS('~'), NS('E'), NS('M'))
		),

		// 4. Правило M — бинарные операции (арифметика, битовые '&'/'|' и сравнения 'c')
		Rule(NS('M'), GRB_ERROR_SERIES + 3,     // Ошибка, неверная операция
			14,

			Rule::Chain(2, TS('+'), NS('E')),
			Rule::Chain(3, TS('+'), NS('E'), NS('M')),

			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),

			Rule::Chain(2, TS('*'), NS('E')),
			Rule::Chain(3, TS('*'), NS('E'), NS('M')),

			Rule::Chain(2, TS('/'), NS('E')),
			Rule::Chain(3, TS('/'), NS('E'), NS('M')),

			Rule::Chain(2, TS('&'), NS('E')),
			Rule::Chain(3, TS('&'), NS('E'), NS('M')),

			Rule::Chain(2, TS('|'), NS('E')),
			Rule::Chain(3, TS('|'), NS('E'), NS('M')),

			Rule::Chain(2, TS('c'), NS('E')),
			Rule::Chain(3, TS('c'), NS('E'), NS('M'))
		),

		// 5. Правило F — список параметров при объявлении функции
		Rule(NS('F'), GRB_ERROR_SERIES + 4,     // Ошибка в параметрах функции
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),

		// 6. Правило W — аргументы при вызове функции
		Rule(NS('W'), GRB_ERROR_SERIES + 5,     // Ошибка в параметрах вызываемой функции
			16,

			Rule::Chain(1, TS('i')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(2, TS('-'), NS('E')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),
			Rule::Chain(2, TS('~'), NS('E')),
			Rule::Chain(3, TS('~'), NS('E'), NS('M')),

			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(4, TS('i'), NS('M'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(4, TS('l'), NS('M'), TS(','), NS('W')),
			Rule::Chain(4, TS('-'), NS('E'), TS(','), NS('W')),
			Rule::Chain(5, TS('-'), NS('E'), NS('M'), TS(','), NS('W')),
			Rule::Chain(4, TS('~'), NS('E'), TS(','), NS('W')),
			Rule::Chain(5, TS('~'), NS('E'), NS('M'), TS(','), NS('W'))
		)
	);
}
