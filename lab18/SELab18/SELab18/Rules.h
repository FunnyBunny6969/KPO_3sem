#pragma once

#include "GRB.h"
#define GRB_ERROR_SERIES 600

namespace GRB
{
	#define NS(n) Rule::Chain::N(n)
	#define TS(n) Rule::Chain::T(n)
	Greibach greibach(NS('S'), TS('$'),         // стартовый символ, дно стека
		3,   //ДОЛЖНО БЫТЬ 6                                   // количество правил
		Rule(NS('S'), GRB_ERROR_SERIES + 0,     // Неверная структура программы
			3,                                  // S->m(M=E;); | tf1(f)(M=E;);5 | m(M=E;);5 | tf1(f)(M=E;
			Rule::Chain(7, TS('m'), TS('{'), TS('r'), NS('E'), TS(';'), ('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 1,     // Ошибочный оператор
			8,                                  // N->dt1;|rf;|1=E;|df1(f);|dt1N|rf;N|1=E;N|df1(f);N
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 2,      // Ошибка в выражении
			8,                                   // E->il|1[O]|1[N]|1[N]|1[N]|1[N]
			Rule::Chain(1, TS('i')),
			Rule::Chain(3, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		)
	);
}