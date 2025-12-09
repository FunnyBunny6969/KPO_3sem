#pragma once

#include "GRB.h"
#define GRB_ERROR_SERIES 600

namespace GRB
{

	Greibach greibach(NS('S'), TS('$'),         // стартовый символ, дно стека
		6,                                      // количество правил
		// Правило S - программа
		Rule(NS('S'), GRB_ERROR_SERIES + 0,     // Неверная структура программы
			3,                                  // S->m{NrE;}; | tfi(F){NrE;};S | m{NrE;};S | tfi(F){NrE;}
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('i'), TS('f'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(9, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S'))
		),

		// Правило N - операторы  
		Rule(NS('N'), GRB_ERROR_SERIES + 1,     // Ошибочный оператор
			8,                                  // N->dti;|rE;|i=E;|dtfi(F);|dtiN|rE;N|i=E;N|dtfi(F);N
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N'))
		),

		// Правило E - выражения
		Rule(NS('E'), GRB_ERROR_SERIES + 2,      // Ошибка в выражении
			8,                                   // E->i|l|(E)|i(W)|iM|lM|i(W)M
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),

		// Правило M - операции (НОВОЕ)
		Rule(NS('M'), GRB_ERROR_SERIES + 3, 2,
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),

		// Правило F - параметры функции (НОВОЕ)  
		Rule(NS('F'), GRB_ERROR_SERIES + 4, 2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),

		// Правило W - аргументы вызова (НОВОЕ)
		Rule(NS('W'), GRB_ERROR_SERIES + 5, 4,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);
}